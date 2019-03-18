HW03
===
This is the hw03 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw03 dir, use:

	* `make` to build.

	* `make clean` to clean the ouput files.

4. Extract `gnu-mcu-eclipse-qemu.zip` into hw03 dir. Under the path of hw03, start emulation with `make qemu`.

	See [Lecture 02 ─ Emulation with QEMU] for more details.

5. The sample is a minimal program for ARM Cortex-M4 devices, which enters `while(1);` after reset. Use gdb to get more details.

	See [ESEmbedded_HW02_Example] for knowing how to do the observation and how to use markdown for taking notes.

# Build Your Own Program

1. Edit main.c.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C Standard library functions like `printf`, `malloc`, etc.

# HW03 Requirements

1. How do C functions pass and return parameters? Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.

2. Modify main.c to observe what you found.

3. You have to state how you designed the observation (code), and how you performed it.

	Just like how you did in HW02.

3. If there are any official data that define the rules, you can also use them as references.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

[Lecture 02 ─ Emulation with QEMU]: http://www.nc.es.ncku.edu.tw/course/embedded/02/#Emulation-with-QEMU
[ESEmbedded_HW02_Example]: https://github.com/vwxyzjimmy/ESEmbedded_HW02_Example

--------------------

**If you volunteer to give the presentation next week, check this.**

--------------------

**★★★ Please take your note here ★★★**


# 實驗內容

透過利用反組譯來觀察在 C code 中是如何return回傳值  


# main.c
```no-highlight
int OneArgAdd(int a,int b){return a+b;}
int MultiArgAdd(int a,int b,int c,int d,int e){return a+b+c+d+e;}  
void reset_handler(void)  
{ 
    int a=1,b=2,c,d,e,f;  
    c=OneArgAdd(a,b);  
    d=OneArgAdd(c,10); 
    e=20;  
    f=MultiArgAdd(a,b,c,d,e);  
    while(1);  
}
```

利用  `arm-none-eabi-objdump -D first_asm.elf ` 來得到反組譯過程

# 反組譯碼

```no-highlight
Disassembly of section .mytext:
00000000 <OneArgAdd-0x8>:
0:	20000100 	andcs	r0, r0, r0, lsl #2
4:	00000051 	andeq	r0, r0, r1, asr r0


00000008 <OneArgAdd>:
8:	b480      	push	{r7}
a:	b083      	sub	sp, #12
c:	af00      	add	r7, sp, #0
e:	6078      	str	r0, [r7, #4]   
10:	6039      	str	r1, [r7, #0]
12:	687a      	ldr	r2, [r7, #4]
14:	683b      	ldr	r3, [r7, #0]
16:	4413      	add	r3, r2
18:	4618      	mov	r0, r3
1a:	370c      	adds	r7, #12
1c:	46bd      	mov	sp, r7
1e:	f85d 7b04 	ldr.w	r7, [sp], #4
22:	4770      	bx	lr

00000024 <MultiArgAdd>:
24:	b480      	push	{r7}
26:	b085      	sub	sp, #20
28:	af00      	add	r7, sp, #0
2a:	60f8      	str	r0, [r7, #12]  
2c:	60b9      	str	r1, [r7, #8]
2e:	607a      	str	r2, [r7, #4]
30:	603b      	str	r3, [r7, #0]
32:	68fa      	ldr	r2, [r7, #12]
34:	68bb      	ldr	r3, [r7, #8]
36:	441a      	add	r2, r3
38:	687b      	ldr	r3, [r7, #4]
3a:	441a      	add	r2, r3
3c:	683b      	ldr	r3, [r7, #0]
3e:	441a      	add	r2, r3
40:	69bb      	ldr	r3, [r7, #24]
42:	4413      	add	r3, r2
44:	4618      	mov	r0, r3
46:	3714      	adds	r7, #20
48:	46bd      	mov	sp, r7
4a:	f85d 7b04 	ldr.w	r7, [sp], #4
4e:	4770      	bx	lr

00000050 <reset_handler>:
50:	b580      	push	{r7, lr}
52:	b088      	sub	sp, #32
54:	af02      	add	r7, sp, #8  
56:	2301      	movs	r3, #1
58:	617b      	str	r3, [r7, #20] 
5a:	2302      	movs	r3, #2
5c:	613b      	str	r3, [r7, #16]
5e:	6978      	ldr	r0, [r7, #20]
60:	6939      	ldr	r1, [r7, #16]
62:	f7ff ffd1 	bl	8 <OneArgAdd>
66:	60f8      	str	r0, [r7, #12]
68:	68f8      	ldr	r0, [r7, #12]
6a:	210a      	movs	r1, #10
6c:	f7ff ffcc 	bl	8 <OneArgAdd>
70:	60b8      	str	r0, [r7, #8]
72:	2314      	movs	r3, #20
74:	607b      	str	r3, [r7, #4]
76:	687b      	ldr	r3, [r7, #4]
78:	9300      	str	r3, [sp, #0]
7a:	6978      	ldr	r0, [r7, #20]
7c:	6939      	ldr	r1, [r7, #16]
7e:	68fa      	ldr	r2, [r7, #12]
80:	68bb      	ldr	r3, [r7, #8]
82:	f7ff ffcf 	bl	24 <MultiArgAdd>
86:	6038      	str	r0, [r7, #0]
88:	e7fe      	b.n	88 <reset_handler+0x38>
8a:	bf00      	nop
```

整個程式的反組譯碼可分為三塊
1. 副程式1
2. 副程式2
3. 主程式

在每個區塊的開頭都有`push	{r7}`，而有時會多`push {lr}`，在push完後，會將sp往下減；在程式執行中會將變數存在`sp往下減的地址`與`sp未減前的地址`的空白記憶體中，由先給定初始值的參數先給定位址，等到程式又呼叫到該參數時取出，如下
```
56:	2301      	movs	r3, #1
58:	617b      	str	r3, [r7, #20] 
5a:	2302      	movs	r3, #2
5c:	613b      	str	r3, [r7, #16]
```
在呼叫副函式前，會將r0~r3儲存為即將帶入副函式的引數，如下
```
5e:	6978      	ldr	r0, [r7, #20]
60:	6939      	ldr	r1, [r7, #16]
62:	f7ff ffd1 	bl	8 <OneArgAdd>
```
```
68:	68f8      	ldr	r0, [r7, #12]
6a:	210a      	movs	r1, #10
6c:	f7ff ffcc 	bl	8 <OneArgAdd>
```
```
78:	9300      	str	r3, [sp, #0]
7a:	6978      	ldr	r0, [r7, #20]
7c:	6939      	ldr	r1, [r7, #16]
7e:	68fa      	ldr	r2, [r7, #12]
80:	68bb      	ldr	r3, [r7, #8]
82:	f7ff ffcf 	bl	24 <MultiArgAdd>
```
在第三次呼叫副函式中，由於副函式引數大於4個，所以會將第五個引數儲存到目前sp的地址中(0x200000d8)，以用於傳遞第五個引數，接下來要討論副函式是如何回傳值的，第一個副函式如下
```
00000008 <OneArgAdd>:
8:	b480      	push	{r7}
a:	b083      	sub	sp, #12
c:	af00      	add	r7, sp, #0
e:	6078      	str	r0, [r7, #4]   
10:	6039      	str	r1, [r7, #0]
12:	687a      	ldr	r2, [r7, #4]
14:	683b      	ldr	r3, [r7, #0]
16:	4413      	add	r3, r2
18:	4618      	mov	r0, r3
1a:	370c      	adds	r7, #12
1c:	46bd      	mov	sp, r7
1e:	f85d 7b04 	ldr.w	r7, [sp], #4
22:	4770      	bx	lr
```
第一個副函式的執行過程為
1. 首先 `push {r7}`，避免引響程式運算
2. 將目前sp位置向下推12(0x200000c8)，以利於副函式中的區域變數儲存，再將向下推後之值(0x200000c8)存至r7
3. 利用事前準備好的r0、r1暫存器中的值存存到Step2製造的空白記憶體中
4. 執行副程式中所含的指令(此段為將兩個暫存器值相加)
5. 由存到Step3中存到空白記憶體中的值取出給r2、r3，再互相相加
6. 將最後結果存到r0
7. r7 加 12，再把r7值存給sp
8. 最後把r7、sp值回歸復到主程式的時候
9. 跳回主程式繼續執行
而每一次跳回主程式後都會先將副程式結果儲存到主程式的空白記憶體中，而每次都儲存在上次所儲存的最後值的向下推4的位置

再來看第二個副函式的實行過程
```
00000024 <MultiArgAdd>:
24:	b480      	push	{r7}
26:	b085      	sub	sp, #20
28:	af00      	add	r7, sp, #0
2a:	60f8      	str	r0, [r7, #12]  
2c:	60b9      	str	r1, [r7, #8]
2e:	607a      	str	r2, [r7, #4]
30:	603b      	str	r3, [r7, #0]
32:	68fa      	ldr	r2, [r7, #12]
34:	68bb      	ldr	r3, [r7, #8]
36:	441a      	add	r2, r3
38:	687b      	ldr	r3, [r7, #4]
3a:	441a      	add	r2, r3
3c:	683b      	ldr	r3, [r7, #0]
3e:	441a      	add	r2, r3
40:	69bb      	ldr	r3, [r7, #24]
42:	4413      	add	r3, r2
44:	4618      	mov	r0, r3
46:	3714      	adds	r7, #20
48:	46bd      	mov	sp, r7
4a:	f85d 7b04 	ldr.w	r7, [sp], #4
4e:	4770      	bx	lr
```
第二個副函式的執行過程為
1. 首先 `push {r7}`，避免引響程式運算
2. 將目前sp位置向下推20(0x200000c0)，以利於副函式中的區域變數儲存，再將向下推後之值(0x200000c0)存至r7
3. 利用事前準備好的r0~r3暫存器中的值存存到Step2製造的空白記憶體中
4. 執行副函式指令
5. 依照順序取出兩個暫存器加在一起，由r0依序加到r3，最後再加第五個由sp所存的值(其中只有在加第五個引數時，所和之值存到r3)
6. 最後的結果存到r0
7. r7 加 20，再把r7值存給sp
8. 最後把r7、sp值回歸復到主程式的時候
9. 跳回主程式繼續執行

最後到主程式時會發現整個程式執行時所存的變數是依序儲存的  
0x200000f4存0x00000001  
0x200000f0存0x00000002  
0x200000ec存0x00000003  
0x200000e8存0x0000000d  
0x200000e4存0x00000014  
0x200000e0存0x00000027  

# 結果與討論
1. 呼叫副程式之前會將引數存到r0~r3的暫存器中，多餘引數會由sp代為傳之
2. R3常用於計算資料
3. R7常用於存放SP與進行SP的移位與復位 
4. 若有回傳資料會置於r0中進行回傳，下一行存取其值



