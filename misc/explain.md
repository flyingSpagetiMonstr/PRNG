<!-- [See xxx](#...) -->

# Pre
- 有时将PRNG看作一个**状态机**会很有用
  - 给定状态机初态`state`，则PRNG的工作过程可以被描述为：
    ```python
    while(1):
        bit = g(state)
        yield bit

        state = update(state)
    ```
  - `update`每次改变/更新`state`，`g`从`state`中提出信息（`bit`）作为输出
  - 关键在于更新函数的设计/结构

- 函数/映射选取
  - 伪随机数生成算法很关键的一点是复杂度，若不在乎复杂度，大可计算$e^{x}$，$x$作为种子，$e^{x}$的二进制表示作为输出流，也会有不错的结果(参考测试集中`.\data\data.e`、`.\data\data.pi`等)
  - 因此设计中使用计算机原生的运算、操作作为映射：
    1. 各种算术运算，结构简单的数据到数据映射
       - `add`
       - `xor (bitwise)`
       - `cyclic shift`
       - `not (bitwise)`
    2. 其它不能保持Uniformity的算术运算(不采用为主干运算，仅作辅助)[(Details)](#...)
       - `multiply`
       - `and (bitwise)`
       - `or (bitwise)`
    3. 数据映射到数据，将数据作为数据的地址：数组
       - `x = f[x]`
    4. 数据映射到操作，将数据作为指令的地址：跳转表
       - `operation = operations[x]`

<!-- 避免状态收敛 -->
# Procedure
- 状态统一存在一个结构体里<a id="code-00-back"></a>[(See code)](#code-00)

- `state`、`g`的选取<a id="code-01-back"></a>[(See code)](#code-01)
  - 状态主体选择为一个数组`uint8_t f[256]`，这样状态本身除了可以看作数据，同时可以看作一个$[0, 256]->[0, 256]$的映射。
  - 取`uint8_t i`，`f[i]`作为每次迭代的输出，输出后令`i = f[i]`
  - 易知，若`f`不变，则`i`一旦回到之前的某个值，那么接下来输出的序列将完全重复，故每次`f[i]`使用后即丢弃（更新）

- `update`的设计<a id="code-02-back"></a>[(See code)](#code-02)

  - `PHI`
    - `phi`为函数指针数组，存有`{add, xor, rshitf, unarys}`四个双目运算
    - note: 
      - `rshift`为循环移位
      - unarys中有两个单目运算，用第二个操作数a来选择对x进行哪个单目运算
    - 以`b = PHI(c)(b, f[a])`为例，用`c`选择实施的运算，`b`作主操作数，`f[c]`作副操作数，运算结果赋给`b`
    - 后面的步骤可以看到`a`、`b`、`c`轮换位置，再将其值作不同用法
    <!-- - 这里一系列操作使用了之前提到的所有运算 -->

  - `state->x`：`x`为常量偏移，在每次update时也会自更新一次，设计上是采用最大原根每次作乘法遍历`P=1567`的简化剩余系(1~1566)
    - 为什么`P=1567`？
      - 除了直接用`x`作常量偏移，也有取`f[x]`使用，故需要有较长的周期，避免因周期过短在`f[]`未充分更新时即开始循环，会导致`f[x]`出现周期性
      - 每次迭代，`f[]`的哪个元素会被更新是随机的，理想情况下，`x`周期应长于`f[]`全部更新所需轮数
      - 参考[Coupon collector's problem](https://en.wikipedia.org/wiki/Coupon_collector%27s_problem)，即：每次从1~n随机取1个值，平均需要取多少次才能遍历1~n？问题的解由以下公式给出：<br>
        $E[n] = n \cdot \left(1 + \frac{1}{2} + \frac{1}{3} + \ldots + \frac{1}{n}\right)$
      - $E[256] = 1567.832310$，取最近的素数即得1567
    - 为什么取最大原根？
      - 每次乘的数更大可以在模`P`后有更好的Uniformity
  - pass

<!-- ================================================== -->
# Codes

## Code-00

```c
// state = {f[], x, i}
typedef struct _state_t
{
    uint8_t f[LEN];
    uint8_t x;
    uint8_t i;
} state_t;
...
// in main():
state_t _s = {0};
state_t *state = &_s;
```
[Go back](#code-00-back)

## Code-01
```c
#define IS_ODD(x) ((x)&1)
#define LEN 256
...
// in main():
uint8_t i = 0;
for (uint64_t cnt = 0; cnt < stream_len; cnt++)
{
    j = f[i];
    // yield j
    update(f, i);
    i = j;
}
```
[Go back](#code-01-back)


## Code-02
- defines for `PHI`: 
  ```c
  // cyclic rshift for uint8_t
  #define RSHIFT(x, n) ((uint8_t)(((x)>>(n))^((x)<<(8-(n))))) 

  #define OP_N (4) // 2^{2}
  #define TO_FOUR (0b11)
  #define TO_EIGHT (0b111)

  state_t *auxiliary = NULL; // auxiliary = state;
  uint8_t add(uint8_t x, uint8_t a) {return x + a;}
  uint8_t xor(uint8_t x, uint8_t a) {return x ^ a;} 
  uint8_t rshitf(uint8_t x, uint8_t a) {return RSHIFT(x, a&TO_EIGHT);}
  uint8_t unarys(uint8_t x, uint8_t a) {return a? ~x: auxiliary->f[x];}

  typedef uint8_t (*operation)(uint8_t, uint8_t);
  operation phi[OP_N] = {add, xor, rshitf, unarys};
  #define PHI(x) (phi[(x)&TO_FOUR])
  ```

- update:
  ```c
  uint8_t old = f[index];

  uint8_t new = 0;
  uint8_t register a = index;
  uint8_t register b = f[COMPRESS(a + state->x)];
  uint8_t register c = f[COMPRESS(state->x)];
  for (int cnt = 0; cnt < 22; cnt++)
  {
      a = PHI(b)(a, c);
      b = PHI(c)(b, f[a]);
      c = PHI(f[a])(c, b);
      f[b] += c;
      f[c] += b;
  }
  new = a;

  new += state->x;
  state->x = GRNG_ITER(state->x);

  new += (new == old);

  f[index] = new;
  
  return;
  ```
[Go back](#code-02-back)




