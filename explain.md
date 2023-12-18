# note: this document is for the initial version

# Pre
- 有时将PRNG看作一个**状态机**会很有用
  - 给定状态机初态$state$，则PRNG的工作过程可以被描述为：
    ```python
    while(1):
        bit = g(state)
        yield bit

        state = f(state)
    ```
  - `g`为生成/提取函数，每次从`state`中提出信息作为输出比特
  - `f`为更新函数，每次改变state

- 在

# How it works

<!-- ## 流程 -->

- 定义一个数组`uint8_t f[256]`，则`f`可看做 0~256 -> 0~256 的一个映射（同时，`f`可以看作该PRNG的种子）

- <a id="code-01-back"></a>每次迭代将`i`经`f`映射，取`f[i]`的奇偶性（即最低位比特）作为输出`bit`，并令`i = f[i]`[(See code)](#code-01)

- 伪随机性完全依赖于`f`，重点在于`f`的维护以及避免状态收敛
  - 易知，在`f`不变的情况下一旦`i`回到之前的某个值，那么接下来生成的序列将完全重复，故每次`f[i]`使用后即丢弃（更新）[(See code)](#code-02)

  - 更新方法：$f(i_{new}):=\Sigma_{x=0}^{255}(f(f(x))\gg1)$<br>
    更新时实际上会发生`uint8_t`的整型溢出，溢出位直接舍弃即可（即不作任何处理）[(See code)](#code-03)

  - 为什么这样更新？
    1. 输出`bit`由$f(i_{old})$的最低位决定，故更新时右移丢弃最低位
    2. 
<!-- ================================================== -->
# Codes
## Code-01
```c
#define IS_ODD(x) ((x)&1)
#define LEN 256
...

i = 0;
for (uint64_t cnt = 0; cnt < stream_len; cnt++)
{
    j = f[i];
    bit = IS_ODD(j);
    i = j;
}
```
[Go back](#code-01-back)

## Code-02
```c
...
bit = IS_ODD(j);
update(f, i);
i = j;
...
```
## Code-03
```c
int new = 0;
for (int cnt = 0; cnt < LEN; cnt++)
{
    new += f[f[cnt]]>>1;
}
f[i] = new;
```

<!-- ================================================== -->
```c
// state = {f[], bitstream[], x, i}
typedef struct _state_t
{
    uint8_t f[LEN];
    uint8_t bitstream[LEN];
    uint8_t x;
    uint8_t i;
    // uint64_t stream_len;
} state_t;
```