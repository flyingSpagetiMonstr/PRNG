# Preliminaries
### 有时将PRNG看作一个**状态机**会很有用
  - 给定状态机初态`state`，则PRNG的工作过程可以被描述为：
    ```python
    while(1):
        bit = g(state)
        yield bit

        state = update(state)
    ```
  - `update`每次改变/更新`state`，`g`从`state`中提出信息（`bit`）作为输出
  - 关键在于更新函数的设计/结构

### Key
PRNG的关键在于：
- 保证Uniformity，即零和一出现的概率相等
- 避免短周期（收敛），理想情况下，状态应具有遍历性
- 复杂性
  - 经过足够多（复杂）的变换，使新状态（下一状态）和旧状态（当前状态）之间呈现伪独立性
  - 采用足够复杂（难以被识别）的结构，使生成的比特流呈现伪无规律

### 函数/映射选取
  - 伪随机数生成算法很关键的一点是复杂度，若不在乎复杂度，大可计算$e^{x}$，$x$作为种子，$e^{x}$的二进制表示作为输出流，也会有不错的结果(参考测试集中`.\data\data.e`、`.\data\data.pi`等)
  - 因此设计中使用计算机原生的运算、操作作为映射，按结构可分为：
    1. 各种算术运算，结构简单的数据到数据映射
       - `add`
       - `xor (bitwise)`
       - `cyclic shift`
       - `not (bitwise)`
    2. 其它不能保持Uniformity的算术运算(不采用为主干运算，仅作辅助)<a id="f-02-back"></a>[[See more @f-02]](#f-02)
       - `multiply`
       - `and (bitwise)`
       - `or (bitwise)`
    3. 数据映射到数据，将数据解释为数据的地址，化数组为映射
       - `x = f[x]`
    4. 数据映射到操作，将数据解释为指令的地址：跳转表
       - `operation = operations[x]`

### [Coupon collector's problem](https://en.wikipedia.org/wiki/Coupon_collector%27s_problem)
   - 每次从1 ~ n随机取1个值，平均需要取多少次才能遍历1 ~ n？
   - 问题的解由以下公式给出：<br>
   $E[n] = n \cdot \left(1 + \frac{1}{2} + \frac{1}{3} + \ldots + \frac{1}{n}\right)$

# Procedure
### note
- 代码实现中，状态统一存在一个结构体里<a id="code-00-back"></a>[[See code]](#code-00)

### `state`、`g`的选取<a id="code-01-back"></a>[[See code]](#code-01)
  - 状态主体选择为一个数组`uint8_t f[256]`，这样状态本身除了可以看作数据，同时可以看作一个$[0, 256]->[0, 256]$的映射。
  - 取`uint8_t i`，`f[i]`作为每次迭代的输出，输出后令`i = f[i]`
  - 易知，若`f[]`不变，则`i`一旦回到之前的某个值，那么接下来输出的序列将完全重复，故每次`f[i]`使用后即丢弃（更新）

### `update`的设计<a id="code-02-back"></a>[[See code]](#code-02)
  - 核心
    - `phi`为函数指针数组，存有`{add, xor, rshitf, unarys}`四个双目运算
      - `rshift`为循环移位
      - `unarys`中有两个单目运算，用第二个操作数a来选择对x进行哪个单目运算
    - 三次`PHI`
      - 以`b = PHI(c)(b, f[a])`为例，用`c`选择实施的运算，`b`作主操作数，`f[c]`作副操作数，运算结果赋给`b`
      - 后面的步骤将`a`、`b`、`c`轮换位置，以将其值作不同用处
      - 同时`a`除了自更新时，均以`f[a]`形式参与变换
    - 循环22次后，取`a`值作为新`f[i]`
      - 为什么循环22次？时间原因这里尚未考虑得十分仔细，暂取[$E[8] = 21.742857 \approx 22$](#coupon-collectors-problem)，实测效果很好
    - 总体上，这里一系列操作混杂了之前提到的[所有方法](#函数映射选取)
  - 善后
    - 因为运算结果对`f[]`状态的依赖性比较大，故最后`+=`一个常量偏移[`state->x`](#state-x)
    - 最后，若`f[]`新值和原值相等，则`+=1`<a id="f-01-back"></a>[[See more @f-01]](#f-01)

### `state->x`<a id="code-03-back"></a>[[See code]](#code-03)
  - `x`为常量偏移，在每次update时也会自更新一次，设计上是采用最大原根每次作乘法遍历`P=1567`的简化剩余系(1~1566)
  - 为什么`P=1567`？
    - `x`除了直接作常量偏移，也以`c = f[COMPRESS(state->x)]`等用法来进行“三次`PHI`”前[`a`、`b`、`c`的初始化](#abc的初始化)，故需要有较长的周期，避免因周期过短在`f[]`未充分更新时即开始循环，会导致`f[x]`出现周期性
    - 理想情况下，`x`的周期应长于`f[]`全部更新一遍所需的轮数，而每次迭代，`f[]`的哪个元素会被更新可以看作随机的
    - [$E[256] = 1567.832310$](#coupon-collectors-problem)，取最近的素数即得1567
  - 为什么取最大原根？
    - 每次乘的数更大可以在模`P`后有更好的Uniformity

### `a`、`b`、`c`的初始化
  - PRNG的每次更新所使用的参数中，必然包含之前步骤所得出的“新”值
  - `a`直接取`i`，`b`和`c`则借用了[`state->x`](#state-xsee-code)

### 其它
  1. 更新过程中使用了`a`、`b`、`c`三个变量，而最终只取`a`，在打乱效果较可观的情况下，这样有点浪费计算量，故最后增加了两句根据`b`、`c`更改`f[]`的操作
     ```c
     uint8_t B = 0, C = 0;
     B = PHI(a)(f[c], b|1);
     C = PHI(a)(f[b], c|1);
     f[b] = B;
     f[c] = C;
     ```
<!-- ================================================== -->
# Codes

## Code-00
结构体
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
`state`、`g`的选取
```c
#define IS_ODD(x) ((x)&1)
#define LEN 256
...
// in main():
uint8_t i = 0;
for (uint64_t cnt = 0; cnt < stream_len; cnt++)
{
    j = f[i];
    // fwrite(&j, 1, 1, out);
    update(f, i);
    i = j;
}
```
[Go back](#code-01-back)


## Code-02
defines for `PHI`
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
---
`update`
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
}

uint8_t B = 0, C = 0;
B = PHI(a)(f[c], b|1);
C = PHI(a)(f[b], c|1);
f[b] = B;
f[c] = C;

new = a;

new += state->x;
state->x = GRNG_ITER(state->x);

new += (new == old);

f[index] = new;

return;
```
[Go back](#code-02-back)

## Code-03
`state->x`
```c
// ==================================
// GRNG: Generator Random Number Generator 
// P: 1567, G_most = 1565
#define P 1567
#define G 1565 // maximum primitive root
#define G_MULT(x) (((x)*G)%P)
#define GRNG_ITER(x) (G_MULT(x))
#define COMPRESS(x) ((uint8_t)(x))
// ==================================
...
// in update():
uint8_t register a = index;
uint8_t register b = f[COMPRESS(a + state->x)];
uint8_t register c = f[COMPRESS(state->x)];
...
state->x = GRNG_ITER(state->x);

```
[Go back](#code-03-back)

# Footnotes
## f-02
- note
  - 这里设计的程序，输出是按整数输出（一次输出一个`uint8_t`），主要操作也都是在`uint8_t`上进行
- 以双目运算为例，运算对象是`uint8_t`的情况下，很多双目运算在两个操作数完全随机时，输出是非随机的
  - 比如，最好理解的是移位运算，丢弃移出位，总是会增加0的个数
  - 其它很明显的有除法、取模、按位与、按位或等等
  - 经测试，乘法、以50%概率选择进行“按位与”或“按位或”都不符合，测试代码可参考：
    ```c
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            sum[(uint8_t)(i & j)]++;
            sum[(uint8_t)(i | j)]++;
        }
    }
    for (int index = 0; index < 256; index++)
    {
        cout << index << ": " << sum[index] << endl;
    }
    ```
    最后检查sum数组中值是不是全相等即可（当然也可以不借助程序直接用概率论来算，但这样快一些）
[Go back](#f-02-back)


## f-01
- 在早期的代码中，这避免了更新后`f[k]=k`并一直保持的情况（状态收敛），现在update主干更新算法的结构和混淆都更加复杂，理论上可以去掉这个拐杖了，但由于没时间还没做过相应的测试
[Go back](#f-01-back)
