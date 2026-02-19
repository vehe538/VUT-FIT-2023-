-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2025 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Juraj Mesík <xmesikj00stud.fit.vutbr.cz>
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_INV  : out std_logic;                      -- pozadavek na aktivaci inverzniho zobrazeni (1)
   OUT_WE   : out std_logic;                      -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'

   -- stavove signaly
   READY    : out std_logic;                      -- hodnota 1 znamena, ze byl procesor inicializovan a zacina vykonavat program
   DONE     : out std_logic                       -- hodnota 1 znamena, ze procesor ukoncil vykonavani programu (narazil na instrukci halt)

 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

  signal PC_REG     : std_logic_vector(12 downto 0);
  signal PC_INC     : std_logic;
  signal PC_DEC     : std_logic;
  signal PC_SETZERO : std_logic;

  signal PTR_REG  : std_logic_vector(12 downto 0);
  signal PTR_INC  : std_logic;
  signal PTR_DEC  : std_logic;

  signal CNT_REG      : std_logic_vector(7 downto 0) := (others => '0');
  signal CNT_INC      : std_logic;
  signal CNT_DEC      : std_logic;
  signal CNT_RST      : std_logic;
  signal CNT_ISZERO   : std_logic;

  signal MX1      : std_logic_vector(12 downto 0);
  signal MX1_SEL  : std_logic;

  signal MX2      : std_logic_vector(7 downto 0);
  signal MX2_SEL  : std_logic_vector(1 downto 0) := (others => '0');

  signal TMP         : std_logic_vector(7 downto 0) := (others => '0');
  signal TMP_LOAD    : std_logic;
  signal TMP_LOAD_IN : std_logic;
  signal TMP_CONVERT : std_logic;


  type fsm_state is 
  (
    s_idle,
    s_find_start,
    s_fetch,
    s_decode,
    s_move_right,   -- >
    s_move_left,    -- <
    s_inc,          -- +
    s_inc_w,        -- +
    s_dec,          -- -
    s_dec_w,        -- -
    s_print_wait,   -- .
    s_print,        -- .
    s_read_wait,    -- ,
    s_read,         -- ,
    s_while_start,  -- [
    s_while_start2, -- [
    s_while_start3, -- [
    s_while_start4, -- [
    s_while_start5, -- [
    s_while_end,    -- ]
    s_while_end2,   -- ]
    s_while_end3,   -- ]
    s_while_end4,   -- ]
    s_while_end5,   -- ]
    s_dowhile_start,-- (
    s_dowhile_end,  -- )
    s_convert,      -- 0-9 A-F to 0x[0-9A-F]0
    s_rewrite,      -- 0x[0-9A-F]0 to mem[ptr]
    s_halt,         -- @
    s_done  
  );

  signal current_state : fsm_state;
  signal next_state    : fsm_state;

begin

 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --      - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --      - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly. 

  --program counter PC
  pc_cntr: process (RESET, CLK)
  begin
    if (RESET = '1') then
      PC_REG <= (others=>'0');
    elsif rising_edge(CLK) then
      if (PC_INC = '1') then
        PC_REG <= PC_REG+1;
      elsif (PC_DEC = '1') then
        PC_REG <= PC_REG - 1;
      elsif (PC_SETZERO = '1') then
        PC_REG <= (others=>'0');
      end if;
    end if;
  end process;

  -- helper variable TMP
  tmp_reg: process (RESET, CLK)
  begin
    if (RESET = '1') then
      TMP <= (others=>'0');
    elsif rising_edge(CLK) then
      if (TMP_CONVERT = '1') then
        case DATA_RDATA is
          when X"41" => TMP <= X"A0";
          when X"42" => TMP <= X"B0";
          when X"43" => TMP <= X"C0";
          when X"44" => TMP <= X"D0";
          when X"45" => TMP <= X"E0";
          when X"46" => TMP <= X"F0";
          when X"30" => TMP <= X"00";
          when X"31" => TMP <= X"10";
          when X"32" => TMP <= X"20";
          when X"33" => TMP <= X"30";
          when X"34" => TMP <= X"40";
          when X"35" => TMP <= X"50";
          when X"36" => TMP <= X"60";
          when X"37" => TMP <= X"70";
          when X"38" => TMP <= X"80";
          when X"39" => TMP <= X"90";
          when others => null;
        end case;
      elsif (TMP_LOAD = '1') then
        TMP <= DATA_RDATA; 
      elsif (TMP_LOAD_IN <= '1') then
        TMP <= IN_DATA;
      end if;
    end if;
  end process;


  -- multiplexer mx1
  mux1 : process (CLK, PC_REG, PTR_REG, MX1_SEL)
  begin
    case MX1_SEL is
      when '0' => DATA_ADDR <= PC_REG;
      when '1' => DATA_ADDR <= PTR_REG;
      when others => null;
    end case;
    
  end process;

  -- multiplexer mx2
  mux2 : process (CLK, IN_DATA, TMP, MX2_SEL)
  begin
    case MX2_SEL is
      when "00" => DATA_WDATA <= TMP;
      when "01" => DATA_WDATA <= DATA_RDATA;
      when "10" => DATA_WDATA <= DATA_RDATA-1;
      when "11" => DATA_WDATA <= DATA_RDATA+1;
      when others => null;
    end case;
    
  end process;

  -- memory pointer PTR
  ptr_cntr: process(RESET, CLK)
  begin
    if (RESET = '1') then
      PTR_REG <= (others=>'0');
    elsif (rising_edge(CLK)) then
      elsif(PTR_INC = '1') then
        if (PTR_REG = X"1FFF") then
          PTR_REG <= (others => '0');
        else
          PTR_REG <= PTR_REG + 1;
        end if;
      elsif (PTR_DEC = '1') then
        if (PTR_REG = X"0000") then
          PTR_REG <= (others => '1');
        else
          PTR_REG <= PTR_REG - 1;
        end if;
    end if;
  end process;

  -- 'while' loop brackets counter CNT
  cnt_cntr: process(RESET, CLK)
  begin
    if (RESET = '1') then
      CNT_REG <= (others=>'0');
    elsif(CNT_INC = '1') then
      CNT_REG <= CNT_REG + 1;
    elsif(CNT_DEC = '1') then
      CNT_REG <= CNT_REG - 1;
    elsif (CNT_RST = '1') then
      CNT_REG <= X"01";
    end if;
  end process;
 
  -- process for checking if CNT is zero
  cnt_zerocheck: process(RESET, CLK)
  begin
    if (CNT_REG = X"00") then
      CNT_ISZERO <= '1';
    elsif(CNT_REG > X"00") then
      CNT_ISZERO <= '0';
    else 
      CNT_ISZERO <= '0';
    end if;
  end process;

  -- current state FSM logic
  fsm_pstate: process (RESET, CLK)  
    begin
      if (RESET = '1') then
        current_state <= s_idle;
      elsif (rising_edge(CLK)) then
        if (EN = '1') then
          current_state <= next_state;
        end if;
      end if;
    end process;

  -- next state FSM logic
  fsm_nstate: process (RESET, CLK, EN, DATA_RDATA, CNT_RST, IN_VLD, OUT_BUSY)
    begin

      DATA_RDWR <= '1';
      DATA_EN   <= '0';
      IN_REQ    <= '0';
      OUT_INV   <= '0';
      OUT_WE    <= '0';
      READY     <= '0';
      DONE      <= '0';
      OUT_DATA  <= (others => '0');

      PC_INC    <= '0';
      PC_DEC    <= '0';
      PC_SETZERO <= '0';
    
      TMP_LOAD <= '0';
      TMP_LOAD_IN <= '0';
      TMP_CONVERT <= '0';

      PTR_INC   <= '0';
      PTR_DEC   <= '0';

      CNT_INC   <= '0';
      CNT_DEC   <= '0';
      CNT_RST  <= '0';

      MX1_SEL   <= '0';
      MX2_SEL   <= "01";

      case current_state is

        when s_idle =>

          DATA_EN <= '1';
          DATA_RDWR <= '1';

          if (EN = '1') then

            DATA_RDWR <= '1';
            DATA_EN <= '1';
            next_state <= s_find_start;

          else
            next_state <= s_idle;
          end if;

        when s_find_start => 
            
            if (DATA_RDATA = X"40") then

              DATA_RDWR <= '1';
              DATA_EN <= '1';
              PC_SETZERO <= '1';
              next_state <= s_fetch;
            
            else
              PC_INC <= '1';
              PTR_INC <= '1';
              DATA_RDWR <= '1';
              DATA_EN <= '1';
              next_state <= s_find_start;  
            end if;
        
        when s_fetch =>

            if (EN = '1') then
              DATA_EN <= '1';
              DATA_RDWR <= '1';
              next_state <= s_decode;
            else
              next_state <= s_idle;
            end if;

        when s_decode  =>
          case DATA_RDATA is
            when X"3E" => next_state <= s_move_right;
            when X"3C" => next_state <= s_move_left;
            when X"2B" => next_state <= s_inc;
            when X"2D" => next_state <= s_dec;
            when X"5B" => next_state <= s_while_start;
            when X"5D" => next_state <= s_while_end;
            when X"28" => next_state <= s_dowhile_start;
            when X"29" => next_state <= s_dowhile_end;
            when X"2E" => next_state <= s_print_wait;
            when X"2C" => next_state <= s_read_wait;
            when X"40" => next_state <= s_halt;
            when others =>
              if (DATA_RDATA >= X"30" and DATA_RDATA <= X"39") then
                next_state <= s_convert;
                TMP_LOAD <= '1';
              elsif (DATA_RDATA >= X"41" and DATA_RDATA <= X"46") then
                TMP_LOAD <= '1';
                next_state <= s_convert;
              else 
                PC_INC <= '1';
                next_state <= s_fetch;
              end if;
          end case;

          when s_done => 
                READY <= '1';
                DONE <= '1';
                next_state <= s_done;
          
          when s_move_right =>
            
              PTR_INC <= '1';
              PC_INC <= '1';
              next_state <= s_fetch;

          when s_move_left =>

            PTR_DEC <= '1';
            PC_INC <= '1';
            next_state <= s_fetch;

          when s_inc => 

            DATA_EN <= '1';
            DATA_RDWR <= '1';
            MX1_SEL <= '1';
            next_state <= s_inc_w;
        
          when s_inc_w => 

            DATA_EN <= '1';
            DATA_RDWR <= '0';
            MX2_SEL <= "11";
            MX1_SEL <= '1';
            PC_INC <= '1'; 
            next_state <= s_fetch;

          when s_dec => 
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            MX1_SEL <= '1';
            next_state <= s_dec_w;

          when s_dec_w => 

            DATA_EN <= '1';
            DATA_RDWR <= '0';
            MX2_SEL <= "10";
            MX1_SEL <= '1';
            PC_INC <= '1'; 
            next_state <= s_fetch;

          when s_print_wait => 
            MX1_SEL <= '1';
            if (OUT_BUSY = '1') then
              next_state <= s_print_wait;
            else
              DATA_EN <= '1';
              DATA_RDWR <= '1';
              next_state <= s_print;
            end if;

          when s_print =>
            MX1_SEL <= '1';
            OUT_WE <= '1';
            OUT_DATA <= DATA_RDATA;
            PC_INC <= '1';
            next_state <= s_fetch;

          when s_read_wait =>
            MX1_SEL <= '1';
            IN_REQ <= '1';
            if (IN_VLD <= '0') then
              next_state <= s_read_wait;
            else               
              TMP_LOAD_IN <= '1';
              next_state <= s_read;
            end if;

          when s_read =>

            DATA_EN <= '1';
            DATA_RDWR <= '0';
            MX1_SEL <= '1';
            MX2_SEL <= "00";
            PC_INC <= '1';
            next_state <= s_fetch;

          when s_while_start =>
            PC_INC <= '1';
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            MX1_SEL <= '1';
            next_state <= s_while_start2;

          when s_while_start2 =>

            if (DATA_RDATA = X"00") then
              CNT_RST <= '1';
              next_state <= s_while_start3;
            else 
              
              next_state <= s_fetch;
            end if;
            
          when s_while_start3 =>
              DATA_EN <= '1';
              DATA_RDWR <= '1';
              next_state <= s_while_start4;
          
          when s_while_start4 =>
              
            if (DATA_RDATA = X"5B") then
              CNT_INC <= '1';
            elsif (DATA_RDATA = X"5D") then
              CNT_DEC <= '1';
            end if; 
            next_state <= s_while_start5;

          when s_while_start5 =>
            PC_INC <= '1';
            if (CNT_ISZERO <= '1') then
              next_state <= s_fetch;
            else 
              next_state <= s_while_start3;
            end if;
          
          when s_while_end => 
            MX1_SEL <= '1';
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            next_state <= s_while_end2;
          
          when s_while_end2 =>

            if (DATA_RDATA = X"00") then
              PC_INC <= '1';
              next_state <= s_fetch;
            else            
              CNT_RST <= '1';
              PC_DEC <= '1';
              next_state <= s_while_end3;
            end if;

          when s_while_end3 =>
              DATA_EN <= '1';
              DATA_RDWR <= '1';
              next_state <= s_while_end4;

          when s_while_end4 =>
            if (DATA_RDATA = X"5D") then
              CNT_INC <= '1';
            elsif (DATA_RDATA = X"5B") then
              CNT_DEC <= '1';
            end if;
            next_state <= s_while_end5;
            
          when s_while_end5 =>
            if (CNT_ISZERO = '1') then
              PC_INC <= '1';
              next_state <= s_fetch;
            else 
              PC_DEC <= '1';
              next_state <= s_while_end3;
            end if;
          
          when s_dowhile_start => 
            next_state <= s_fetch;

          when s_dowhile_end => 
            next_state <= s_fetch;

          when s_convert => 
            
            MX1_SEL <= '1';
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            TMP_CONVERT <= '1';
            MX2_SEL <= "00";
            
            next_state <= s_rewrite;

          when s_rewrite =>
            MX1_SEL <= '1';
            DATA_EN <= '1';
            DATA_RDWR <= '0';
            MX2_SEL <= "00";
            PC_INC <= '1';
            next_state <= s_fetch;

          when s_halt => 
            DONE <= '0';
            READY <= '1';
            PC_INC <= '1';
            next_state <= s_done;

        end case;
    end process;
          
end behavioral;

