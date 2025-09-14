library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cpu is
  port (
    CLK : in std_logic;
    RESET : in std_logic;
    EN : in std_logic;
    DATA_RDATA : in std_logic_vector (7 downto 0);
    IN_DATA : in std_logic_vector (7 downto 0);
    IN_VLD : in std_logic;
    OUT_BUSY : in std_logic;
    DATA_ADDR : out std_logic_vector (12 downto 0);
    DATA_WDATA : out std_logic_vector (7 downto 0);
    DATA_RDWR : out std_logic;
    DATA_EN : out std_logic;
    IN_REQ : out std_logic;
    OUT_DATA : out std_logic_vector (7 downto 0);
    OUT_INV : out std_logic;
    OUT_WE : out std_logic;
    READY : out std_logic;
    DONE : out std_logic);
end entity cpu;

architecture rtl of cpu is
  signal ptr_reg : std_logic_vector (12 downto 0);
  signal ptr_inc : std_logic;
  signal ptr_dec : std_logic;
  signal pc_reg : std_logic_vector (12 downto 0);
  signal pc_inc : std_logic;
  signal pc_dec : std_logic;
  signal mx1_select : std_logic;
  signal mx2_out : std_logic_vector (7 downto 0);
  signal mx2_select : std_logic_vector (1 downto 0);
  signal state : std_logic_vector (4 downto 0);
  signal nstate : std_logic_vector (4 downto 0);
  signal n19_o : std_logic;
  signal n21_o : std_logic;
  signal n22_o : std_logic_vector (1 downto 0);
  signal n23_o : std_logic_vector (12 downto 0);
  signal n27_o : std_logic;
  signal n29_o : std_logic_vector (7 downto 0);
  signal n31_o : std_logic;
  signal n33_o : std_logic_vector (7 downto 0);
  signal n35_o : std_logic;
  signal n37_o : std_logic;
  signal n38_o : std_logic_vector (3 downto 0);
  signal n40_o : std_logic_vector (7 downto 0);
  signal n45_o : std_logic;
  signal n47_o : std_logic_vector (12 downto 0);
  signal n49_o : std_logic_vector (12 downto 0);
  signal n51_o : std_logic;
  signal n53_o : std_logic_vector (12 downto 0);
  signal n55_o : std_logic_vector (12 downto 0);
  signal n56_o : std_logic_vector (12 downto 0);
  signal n57_o : std_logic_vector (12 downto 0);
  signal n65_o : std_logic_vector (12 downto 0);
  signal n67_o : std_logic_vector (12 downto 0);
  signal n68_o : std_logic_vector (12 downto 0);
  signal n69_o : std_logic_vector (12 downto 0);
  signal n83_o : std_logic;
  signal n86_o : std_logic;
  signal n89_o : std_logic_vector (4 downto 0);
  signal n91_o : std_logic;
  signal n93_o : std_logic;
  signal n95_o : std_logic;
  signal n97_o : std_logic;
  signal n99_o : std_logic;
  signal n101_o : std_logic;
  signal n103_o : std_logic;
  signal n105_o : std_logic;
  signal n107_o : std_logic;
  signal n109_o : std_logic;
  signal n111_o : std_logic;
  signal n112_o : std_logic_vector (8 downto 0);
  signal n115_o : std_logic;
  signal n127_o : std_logic_vector (4 downto 0);
  signal n129_o : std_logic;
  signal n131_o : std_logic;
  signal n133_o : std_logic;
  signal n135_o : std_logic;
  signal n137_o : std_logic;
  signal n139_o : std_logic;
  signal n141_o : std_logic;
  signal n143_o : std_logic;
  signal n145_o : std_logic;
  signal n146_o : std_logic;
  signal n149_o : std_logic;
  signal n152_o : std_logic;
  signal n155_o : std_logic;
  signal n157_o : std_logic_vector (4 downto 0);
  signal n159_o : std_logic;
  signal n161_o : std_logic;
  signal n163_o : std_logic;
  signal n165_o : std_logic_vector (4 downto 0);
  signal n167_o : std_logic;
  signal n169_o : std_logic;
  signal n171_o : std_logic;
  signal n173_o : std_logic;
  signal n176_o : std_logic_vector (4 downto 0);
  signal n178_o : std_logic;
  signal n180_o : std_logic;
  signal n181_o : std_logic;
  signal n183_o : std_logic_vector (4 downto 0);
  signal n184_o : std_logic_vector (7 downto 0);
  signal n187_o : std_logic;
  signal n189_o : std_logic_vector (4 downto 0);
  signal n191_o : std_logic;
  signal n193_o : std_logic;
  signal n195_o : std_logic;
  signal n197_o : std_logic;
  signal n198_o : std_logic_vector (19 downto 0);
  signal n209_o : std_logic;
  signal n222_o : std_logic;
  signal n227_o : std_logic;
  signal n230_o : std_logic_vector (7 downto 0);
  signal n233_o : std_logic;
  signal n240_o : std_logic;
  signal n245_o : std_logic;
  signal n251_o : std_logic;
  signal n256_o : std_logic;
  signal n271_o : std_logic;
  signal n276_o : std_logic;
  signal n291_o : std_logic;
  signal n297_o : std_logic_vector (1 downto 0);
  signal n315_o : std_logic_vector (4 downto 0);
  constant n317_o : std_logic := '0';
  signal n318_q : std_logic_vector (12 downto 0) := "0000000000000";
  signal n319_q : std_logic_vector (12 downto 0) := "0000000000000";
  signal n321_o : std_logic_vector (4 downto 0);
  signal n322_q : std_logic_vector (4 downto 0);
begin
  DATA_ADDR <= n23_o;
  DATA_WDATA <= mx2_out;
  DATA_RDWR <= n209_o;
  DATA_EN <= n222_o;
  IN_REQ <= n227_o;
  OUT_DATA <= n230_o;
  OUT_INV <= n317_o;
  OUT_WE <= n233_o;
  READY <= n240_o;
  DONE <= n245_o;
  -- ../src/cpu.vhd:51:10
  ptr_reg <= n318_q; -- (isignal)
  -- ../src/cpu.vhd:52:10
  ptr_inc <= n251_o; -- (isignal)
  -- ../src/cpu.vhd:53:10
  ptr_dec <= n256_o; -- (isignal)
  -- ../src/cpu.vhd:56:10
  pc_reg <= n319_q; -- (isignal)
  -- ../src/cpu.vhd:57:10
  pc_inc <= n271_o; -- (isignal)
  -- ../src/cpu.vhd:58:10
  pc_dec <= n276_o; -- (isignal)
  -- ../src/cpu.vhd:61:10
  mx1_select <= n291_o; -- (signal)
  -- ../src/cpu.vhd:62:10
  mx2_out <= n40_o; -- (signal)
  -- ../src/cpu.vhd:63:10
  mx2_select <= n297_o; -- (isignal)
  -- ../src/cpu.vhd:89:10
  state <= n322_q; -- (signal)
  -- ../src/cpu.vhd:90:10
  nstate <= n315_o; -- (signal)
  -- ../src/cpu.vhd:98:7
  n19_o <= '1' when mx1_select = '0' else '0';
  -- ../src/cpu.vhd:100:7
  n21_o <= '1' when mx1_select = '1' else '0';
  n22_o <= n21_o & n19_o;
  -- ../src/cpu.vhd:97:5
  with n22_o select n23_o <=
    ptr_reg when "10",
    pc_reg when "01",
    n23_o when others;
  -- ../src/cpu.vhd:110:7
  n27_o <= '1' when mx2_select = "00" else '0';
  -- ../src/cpu.vhd:113:31
  n29_o <= std_logic_vector (unsigned (DATA_RDATA) - unsigned'("00000001"));
  -- ../src/cpu.vhd:112:7
  n31_o <= '1' when mx2_select = "01" else '0';
  -- ../src/cpu.vhd:115:31
  n33_o <= std_logic_vector (unsigned (DATA_RDATA) + unsigned'("00000001"));
  -- ../src/cpu.vhd:114:7
  n35_o <= '1' when mx2_select = "10" else '0';
  -- ../src/cpu.vhd:116:7
  n37_o <= '1' when mx2_select = "11" else '0';
  n38_o <= n37_o & n35_o & n31_o & n27_o;
  -- ../src/cpu.vhd:109:5
  with n38_o select n40_o <=
    DATA_RDATA when "1000",
    n33_o when "0100",
    n29_o when "0010",
    IN_DATA when "0001",
    "00000000" when others;
  -- ../src/cpu.vhd:131:20
  n45_o <= '1' when ptr_reg = "1111111111111" else '0';
  -- ../src/cpu.vhd:134:30
  n47_o <= std_logic_vector (unsigned (ptr_reg) + unsigned'("0000000000001"));
  -- ../src/cpu.vhd:131:9
  n49_o <= n47_o when n45_o = '0' else "1000000000000";
  -- ../src/cpu.vhd:138:20
  n51_o <= '1' when ptr_reg = "1000000000000" else '0';
  -- ../src/cpu.vhd:141:30
  n53_o <= std_logic_vector (unsigned (ptr_reg) - unsigned'("0000000000001"));
  -- ../src/cpu.vhd:138:9
  n55_o <= n53_o when n51_o = '0' else "1111111111111";
  -- ../src/cpu.vhd:137:7
  n56_o <= ptr_reg when ptr_dec = '0' else n55_o;
  -- ../src/cpu.vhd:130:7
  n57_o <= n56_o when ptr_inc = '0' else n49_o;
  -- ../src/cpu.vhd:154:26
  n65_o <= std_logic_vector (unsigned (pc_reg) + unsigned'("0000000000001"));
  -- ../src/cpu.vhd:156:26
  n67_o <= std_logic_vector (unsigned (pc_reg) - unsigned'("0000000000001"));
  -- ../src/cpu.vhd:155:7
  n68_o <= pc_reg when pc_dec = '0' else n67_o;
  -- ../src/cpu.vhd:153:7
  n69_o <= n68_o when pc_inc = '0' else n65_o;
  -- ../src/cpu.vhd:200:25
  n83_o <= '1' when unsigned (DATA_RDATA) <= unsigned'("01000000") else '0';
  -- ../src/cpu.vhd:200:11
  n86_o <= '0' when n83_o = '0' else '1';
  -- ../src/cpu.vhd:200:11
  n89_o <= "00000" when n83_o = '0' else "00001";
  -- ../src/cpu.vhd:196:9
  n91_o <= '1' when state = "00000" else '0';
  -- ../src/cpu.vhd:209:9
  n93_o <= '1' when state = "00001" else '0';
  -- ../src/cpu.vhd:220:13
  n95_o <= '1' when DATA_RDATA = "00111110" else '0';
  -- ../src/cpu.vhd:222:13
  n97_o <= '1' when DATA_RDATA = "00111100" else '0';
  -- ../src/cpu.vhd:224:13
  n99_o <= '1' when DATA_RDATA = "00101011" else '0';
  -- ../src/cpu.vhd:226:13
  n101_o <= '1' when DATA_RDATA = "00101101" else '0';
  -- ../src/cpu.vhd:228:13
  n103_o <= '1' when DATA_RDATA = "01011011" else '0';
  -- ../src/cpu.vhd:230:13
  n105_o <= '1' when DATA_RDATA = "01011101" else '0';
  -- ../src/cpu.vhd:232:13
  n107_o <= '1' when DATA_RDATA = "00101110" else '0';
  -- ../src/cpu.vhd:234:13
  n109_o <= '1' when DATA_RDATA = "00101100" else '0';
  -- ../src/cpu.vhd:236:13
  n111_o <= '1' when DATA_RDATA = "01000000" else '0';
  n112_o <= n111_o & n109_o & n107_o & n105_o & n103_o & n101_o & n99_o & n97_o & n95_o;
  -- ../src/cpu.vhd:219:11
  with n112_o select n115_o <=
    '1' when "100000000",
    '1' when "010000000",
    '1' when "001000000",
    '1' when "000100000",
    '1' when "000010000",
    '1' when "000001000",
    '1' when "000000100",
    '1' when "000000010",
    '1' when "000000001",
    '1' when others;
  -- ../src/cpu.vhd:219:11
  with n112_o select n127_o <=
    "10010" when "100000000",
    "10000" when "010000000",
    "01110" when "001000000",
    "01101" when "000100000",
    "01101" when "000010000",
    "00101" when "000001000",
    "00011" when "000000100",
    "01001" when "000000010",
    "00111" when "000000001",
    "10011" when others;
  -- ../src/cpu.vhd:214:9
  n129_o <= '1' when state = "00010" else '0';
  -- ../src/cpu.vhd:244:9
  n131_o <= '1' when state = "00111" else '0';
  -- ../src/cpu.vhd:249:9
  n133_o <= '1' when state = "01000" else '0';
  -- ../src/cpu.vhd:253:9
  n135_o <= '1' when state = "01001" else '0';
  -- ../src/cpu.vhd:260:9
  n137_o <= '1' when state = "01010" else '0';
  -- ../src/cpu.vhd:264:9
  n139_o <= '1' when state = "00011" else '0';
  -- ../src/cpu.vhd:272:9
  n141_o <= '1' when state = "00100" else '0';
  -- ../src/cpu.vhd:279:9
  n143_o <= '1' when state = "00101" else '0';
  -- ../src/cpu.vhd:285:9
  n145_o <= '1' when state = "00110" else '0';
  -- ../src/cpu.vhd:295:21
  n146_o <= not IN_VLD;
  -- ../src/cpu.vhd:297:11
  n149_o <= '0' when IN_VLD = '0' else '1';
  -- ../src/cpu.vhd:297:11
  n152_o <= '0' when IN_VLD = '0' else '1';
  -- ../src/cpu.vhd:297:11
  n155_o <= '0' when IN_VLD = '0' else '1';
  -- ../src/cpu.vhd:297:11
  n157_o <= nstate when IN_VLD = '0' else "10001";
  -- ../src/cpu.vhd:295:11
  n159_o <= n149_o when n146_o = '0' else '0';
  -- ../src/cpu.vhd:295:11
  n161_o <= n152_o when n146_o = '0' else '0';
  -- ../src/cpu.vhd:295:11
  n163_o <= n155_o when n146_o = '0' else '0';
  -- ../src/cpu.vhd:295:11
  n165_o <= n157_o when n146_o = '0' else "10000";
  -- ../src/cpu.vhd:293:9
  n167_o <= '1' when state = "10000" else '0';
  -- ../src/cpu.vhd:306:9
  n169_o <= '1' when state = "10001" else '0';
  -- ../src/cpu.vhd:311:9
  n171_o <= '1' when state = "01011" else '0';
  -- ../src/cpu.vhd:318:25
  n173_o <= '1' when DATA_RDATA = "00000000" else '0';
  -- ../src/cpu.vhd:318:11
  n176_o <= "00001" when n173_o = '0' else "01100";
  -- ../src/cpu.vhd:317:9
  n178_o <= '1' when state = "01101" else '0';
  -- ../src/cpu.vhd:324:9
  n180_o <= '1' when state = "01100" else '0';
  -- ../src/cpu.vhd:333:23
  n181_o <= not OUT_BUSY;
  -- ../src/cpu.vhd:337:11
  n183_o <= nstate when OUT_BUSY = '0' else "01110";
  -- ../src/cpu.vhd:333:11
  n184_o <= n230_o when n181_o = '0' else DATA_RDATA;
  -- ../src/cpu.vhd:333:11
  n187_o <= '0' when n181_o = '0' else '1';
  -- ../src/cpu.vhd:333:11
  n189_o <= n183_o when n181_o = '0' else "01111";
  -- ../src/cpu.vhd:329:9
  n191_o <= '1' when state = "01110" else '0';
  -- ../src/cpu.vhd:341:9
  n193_o <= '1' when state = "01111" else '0';
  -- ../src/cpu.vhd:347:9
  n195_o <= '1' when state = "10010" else '0';
  -- ../src/cpu.vhd:354:9
  n197_o <= '1' when state = "10011" else '0';
  n198_o <= n197_o & n195_o & n193_o & n191_o & n180_o & n178_o & n171_o & n169_o & n167_o & n145_o & n143_o & n141_o & n139_o & n137_o & n135_o & n133_o & n131_o & n129_o & n93_o & n91_o;
  -- ../src/cpu.vhd:194:7
  with n198_o select n209_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    '0' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    n159_o when "00000000100000000000",
    '1' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '1' when "00000000000010000000",
    '0' when "00000000000001000000",
    '0' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '1' when "00000000000000000100",
    '0' when "00000000000000000010",
    '1' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n222_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    '1' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '1' when "00000010000000000000",
    '0' when "00000001000000000000",
    n161_o when "00000000100000000000",
    '1' when "00000000010000000000",
    '1' when "00000000001000000000",
    '1' when "00000000000100000000",
    '1' when "00000000000010000000",
    '0' when "00000000000001000000",
    '1' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '1' when "00000000000000000100",
    '0' when "00000000000000000010",
    '1' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n227_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    '0' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    '1' when "00000000100000000000",
    '0' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '0' when "00000000000010000000",
    '0' when "00000000000001000000",
    '0' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '0' when "00000000000000000100",
    '0' when "00000000000000000010",
    '0' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n230_o <=
    n230_o when "10000000000000000000",
    n230_o when "01000000000000000000",
    n230_o when "00100000000000000000",
    n184_o when "00010000000000000000",
    n230_o when "00001000000000000000",
    n230_o when "00000100000000000000",
    n230_o when "00000010000000000000",
    n230_o when "00000001000000000000",
    n230_o when "00000000100000000000",
    n230_o when "00000000010000000000",
    n230_o when "00000000001000000000",
    n230_o when "00000000000100000000",
    n230_o when "00000000000010000000",
    n230_o when "00000000000001000000",
    n230_o when "00000000000000100000",
    n230_o when "00000000000000010000",
    n230_o when "00000000000000001000",
    n230_o when "00000000000000000100",
    n230_o when "00000000000000000010",
    n230_o when "00000000000000000001",
    "XXXXXXXX" when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n233_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    n187_o when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    '0' when "00000000100000000000",
    '0' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '0' when "00000000000010000000",
    '0' when "00000000000001000000",
    '0' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '0' when "00000000000000000100",
    '0' when "00000000000000000010",
    '0' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n240_o <=
    '0' when "10000000000000000000",
    '1' when "01000000000000000000",
    '0' when "00100000000000000000",
    '0' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    '0' when "00000000100000000000",
    '0' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '0' when "00000000000010000000",
    '0' when "00000000000001000000",
    '0' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '1' when "00000000000000000100",
    '1' when "00000000000000000010",
    '0' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n245_o <=
    '0' when "10000000000000000000",
    '1' when "01000000000000000000",
    '0' when "00100000000000000000",
    '0' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    '0' when "00000000100000000000",
    '0' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '0' when "00000000000010000000",
    '0' when "00000000000001000000",
    '0' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '0' when "00000000000000000100",
    '0' when "00000000000000000010",
    '0' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n251_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    '0' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    '0' when "00000000100000000000",
    '0' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '0' when "00000000000010000000",
    '0' when "00000000000001000000",
    '0' when "00000000000000100000",
    '0' when "00000000000000010000",
    '1' when "00000000000000001000",
    n115_o when "00000000000000000100",
    '0' when "00000000000000000010",
    '1' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n256_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    '0' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    '0' when "00000000100000000000",
    '0' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '0' when "00000000000010000000",
    '0' when "00000000000001000000",
    '1' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '0' when "00000000000000000100",
    '0' when "00000000000000000010",
    '0' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n271_o <=
    '1' when "10000000000000000000",
    '0' when "01000000000000000000",
    '1' when "00100000000000000000",
    '0' when "00010000000000000000",
    '1' when "00001000000000000000",
    '0' when "00000100000000000000",
    '1' when "00000010000000000000",
    '1' when "00000001000000000000",
    '0' when "00000000100000000000",
    '1' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '1' when "00000000000010000000",
    '1' when "00000000000001000000",
    '0' when "00000000000000100000",
    '1' when "00000000000000010000",
    '1' when "00000000000000001000",
    '0' when "00000000000000000100",
    '0' when "00000000000000000010",
    n86_o when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n276_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    '0' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '0' when "00000010000000000000",
    '0' when "00000001000000000000",
    '0' when "00000000100000000000",
    '0' when "00000000010000000000",
    '0' when "00000000001000000000",
    '0' when "00000000000100000000",
    '0' when "00000000000010000000",
    '0' when "00000000000001000000",
    '0' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '0' when "00000000000000000100",
    '0' when "00000000000000000010",
    '0' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n291_o <=
    '0' when "10000000000000000000",
    '0' when "01000000000000000000",
    '0' when "00100000000000000000",
    '1' when "00010000000000000000",
    '0' when "00001000000000000000",
    '0' when "00000100000000000000",
    '1' when "00000010000000000000",
    '0' when "00000001000000000000",
    n163_o when "00000000100000000000",
    '1' when "00000000010000000000",
    '1' when "00000000001000000000",
    '0' when "00000000000100000000",
    '1' when "00000000000010000000",
    '0' when "00000000000001000000",
    '1' when "00000000000000100000",
    '0' when "00000000000000010000",
    '0' when "00000000000000001000",
    '0' when "00000000000000000100",
    '0' when "00000000000000000010",
    '1' when "00000000000000000001",
    'X' when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n297_o <=
    "00" when "10000000000000000000",
    "00" when "01000000000000000000",
    "00" when "00100000000000000000",
    "00" when "00010000000000000000",
    "00" when "00001000000000000000",
    "00" when "00000100000000000000",
    "00" when "00000010000000000000",
    "00" when "00000001000000000000",
    "00" when "00000000100000000000",
    "01" when "00000000010000000000",
    "00" when "00000000001000000000",
    "10" when "00000000000100000000",
    "00" when "00000000000010000000",
    "00" when "00000000000001000000",
    "00" when "00000000000000100000",
    "00" when "00000000000000010000",
    "00" when "00000000000000001000",
    "00" when "00000000000000000100",
    "00" when "00000000000000000010",
    "00" when "00000000000000000001",
    "XX" when others;
  -- ../src/cpu.vhd:194:7
  with n198_o select n315_o <=
    "00001" when "10000000000000000000",
    "10010" when "01000000000000000000",
    "00001" when "00100000000000000000",
    n189_o when "00010000000000000000",
    "00001" when "00001000000000000000",
    n176_o when "00000100000000000000",
    "01101" when "00000010000000000000",
    "00001" when "00000001000000000000",
    n165_o when "00000000100000000000",
    "00001" when "00000000010000000000",
    "00110" when "00000000001000000000",
    "00001" when "00000000000100000000",
    "00100" when "00000000000010000000",
    "00001" when "00000000000001000000",
    "01010" when "00000000000000100000",
    "00001" when "00000000000000010000",
    "00001" when "00000000000000001000",
    n127_o when "00000000000000000100",
    "00010" when "00000000000000000010",
    n89_o when "00000000000000000001",
    "XXXXX" when others;
  -- ../src/cpu.vhd:129:5
  process (CLK, RESET)
  begin
    if RESET = '1' then
      n318_q <= "0000000000000";
    elsif rising_edge (CLK) then
      n318_q <= n57_o;
    end if;
  end process;
  -- ../src/cpu.vhd:152:5
  process (CLK, RESET)
  begin
    if RESET = '1' then
      n319_q <= "0000000000000";
    elsif rising_edge (CLK) then
      n319_q <= n69_o;
    end if;
  end process;
  -- ../src/cpu.vhd:165:5
  n321_o <= state when EN = '0' else nstate;
  -- ../src/cpu.vhd:165:5
  process (CLK, RESET)
  begin
    if RESET = '1' then
      n322_q <= "00000";
    elsif rising_edge (CLK) then
      n322_q <= n321_o;
    end if;
  end process;
end rtl;

