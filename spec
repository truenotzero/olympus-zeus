6502 notes:
- 8 bit cpu
- 64Kb RAM
- 16bit address bus
- Little Endian

registers:
pc16
sp8, grows downwards (1FF -> 100)
acc8
x
y
f - carry, zero, interrupt disable, decimal mode (bcd), break, overflow, negative

addressing modes:
- implicit
- accumulator (ins a)
- immediate (ins #n)
- zero page (automatic, for the addy range 00-FF)
- zero page, x (sty $10, x - stores y at $10+x)
- zero page, y (similar to above)
- relative (8 bit in the range -126 ~ +129 relative to curr addy)
- absolute (16bit)
- absolute, x (sta mem, x - stores x at mem+x)
- absolute, y (similar to above)
- indirect (dereference, then run the ins)
- indexed indirect (zero page: lda ($40,x) loads a <- $40+x)
- indirect indexed (zero page: lda ($40),y loads a <- ($40)+x)
