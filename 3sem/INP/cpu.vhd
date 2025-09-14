-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2024 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): jmeno <login AT stud.fit.vutbr.cz>
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


  --tmp--
  signal tmp_reg:      std_logic_vector(7 downto 0);
  signal tmp_id:       std_logic_vector(7 downto 0);

  --ptr--
  signal ptr_reg:          std_logic_vector(12 downto 0) := (others => '0');
  signal ptr_inc:          std_logic := '0';
  signal ptr_dec:          std_logic := '0';

  --pc--
  signal pc_reg:       std_logic_vector(12 downto 0) := (others => '0');
  signal pc_inc:       std_logic := '0';
  signal pc_dec:       std_logic := '0';

  --mx--
  signal mx1_out:     std_logic_vector(12 downto 0);
  signal mx1_select : std_logic;
  signal mx2_out:     std_logic_vector(7 downto 0);
  signal mx2_select : std_logic_vector (1 downto 0) := (others => '0');  

  --fsm--
  type fsm_state is (
    init,
    s_start,
    s_fetch, 
    s_cmd, 
    s_value_inc, 
    s_value_inc2,
    s_value_inc3,
    s_value_dec, 
    s_pointer_inc, 
    s_pointer_dec, 
    s_while_start,
    s_while_end, 
    s_while_check, 
    --s_value_to_tmp, --
    --s_tmp_to_value, --
    s_write,
    s_write2, 
    s_read,
    s_read2, 
    s_halt,
    s_others);
  signal state: fsm_state;
  signal nstate: fsm_state;


begin

  mx1: process (mx1_select, pc_reg, ptr_reg) is
  begin
    case mx1_select is
      when '0' => 
        DATA_ADDR <= pc_reg;
      when '1' =>
        DATA_ADDR <= ptr_reg;
      when others =>
    end case;
  end process;


  mx2: process (mx2_select, IN_DATA, DATA_RDATA) is
  begin
    case mx2_select is
      when "00" =>
        mx2_out <= IN_DATA;
      when "01" =>
        mx2_out <= DATA_RDATA - 1;
      when "10" =>
        mx2_out <= DATA_RDATA + 1;
      when "11" =>
        mx2_out <= DATA_RDATA; 
      when others =>
        mx2_out <= (others => '0');
    end case;
  end process; 
  DATA_WDATA <= mx2_out; 


  ptr: process(RESET, CLK, ptr_reg, ptr_inc, ptr_dec) is
  begin
    if (RESET = '1') then
      ptr_reg <= "0000000000000";
    elsif rising_edge(CLK) then
      if ptr_inc = '1' then
        if ptr_reg = "1111111111111" then
          ptr_reg <= "1000000000000";
        else
          ptr_reg <= ptr_reg + 1;
        end if;

      elsif ptr_dec = '1' then
        if ptr_reg = "1000000000000" then
          ptr_reg <= "1111111111111";
        else
          ptr_reg <= ptr_reg - 1;
        end if;
      end if;
    end if;
  end process;


  pc: process(CLK, RESET, pc_inc, pc_dec, pc_reg) is
  begin
    if RESET = '1' then
      pc_reg <= (others => '0');
    elsif rising_edge(CLK) then
      if pc_inc = '1' then
        pc_reg <= pc_reg + 1;
      elsif pc_dec = '1' then
        pc_reg <= pc_reg - 1;
      end if;
    end if;
  end process;
  
  fsm: process (CLK, RESET, DATA_RDATA, EN) is
  begin
    if RESET = '1' then
      state <= init;
    elsif rising_edge(CLK) then
      if EN = '1' then
        state <= nstate;
      end if;
    end if;
  end process;

  fsm_nsl: process(DATA_RDATA, IN_VLD, OUT_BUSY, state) is
    begin
    --init--
      DONE      <= '0';
      READY     <= '0';
      DATA_RDWR <= '0';
      DATA_EN   <= '0';
      IN_REQ    <= '0';
      OUT_WE    <= '0';
      OUT_INV   <= '0';

      ptr_inc   <= '0';
      ptr_dec   <= '0';
      pc_inc    <= '0';
      pc_dec    <= '0';
      mx1_select<= '0';
      mx2_select<= "00";

      --nsl--

      case state is

        when init =>
          DATA_EN <= '1';
          DATA_RDWR <= '1';
          if DATA_RDATA <= x"40" then
            ptr_reg <= pc_reg;
            pc_inc <= '1';
            nstate <= s_start;
          else
            pc_inc <= '1';
            nstate <= init;
          end if;

        when s_start =>
          READY <= '1';       
          nstate <= s_fetch;

        when s_fetch =>
          READY <= '1';
          nstate <= s_cmd;


        when s_cmd =>
          DATA_EN <= '1';
          DATA_RDWR <= '1';
          READY <= '1';
          case DATA_RDATA is
            when x"3E" =>
              nstate <= s_pointer_inc;
            when x"3C" =>
              nstate <= s_pointer_dec;
            when x"2B" =>
              nstate <= s_value_inc;
            when x"2D" =>
              nstate <= s_value_dec;
            when x"5B" =>
              nstate <= s_while_check;
            when x"5D" =>
              nstate <= s_while_check;
            --when x"24" =>
            --  nstate <= s_value_to_tmp;
            --when x"21" =>
            --  nstate <= s_tmp_to_value;
            when x"2E" =>
              nstate <= s_write;
            when x"2C" =>
              nstate <= s_read;
            when x"40" =>
              nstate <= s_halt;
            when others =>
              nstate <= s_others;
          end case;


        when s_pointer_inc =>
          pc_inc <= '1';
          ptr_inc <= '1';
          nstate <= s_fetch;

        when s_pointer_dec =>
          pc_inc <= '1';
          ptr_dec <= '1';
          nstate <= s_fetch;
        
        when s_value_inc =>
          DATA_EN <= '1';
          DATA_RDWR <= '1'; --read mode
          mx1_select <= '1';
          pc_inc <= '1';
          nstate <= s_value_inc2;


        when s_value_inc2 =>
          DATA_EN <= '1';
          DATA_RDWR <= '0'; --write mode
          mx1_select <= '1';
          mx2_select <= "10";
          nstate <= s_fetch;

        when s_value_inc3 =>
          pc_inc <= '1';
          nstate <= s_fetch;

        when s_value_dec =>
          DATA_EN <= '1';
          DATA_RDWR <= '0';
          mx2_select <= "10";
          DATA_RDWR <= '1';
          pc_inc <= '1';
          nstate <= s_fetch;
        
        when s_read =>      
          IN_REQ <= '1';    
          if IN_VLD = '0' then
            nstate <= s_read;
          elsif IN_VLD = '1' then
            DATA_EN <= '1';
            DATA_RDWR <= '1';
            mx1_select <= '1';
            mx2_select <= "00";
            nstate <= s_read2;
          end if;

        
        when s_read2 =>
          mx1_select <= '0';
          pc_inc <= '1';
          nstate <= s_fetch;

        when s_while_start =>
          DATA_EN <= '1';
          pc_inc <= '1';
          mx1_select <= '1';
          nstate <= s_while_check;
        
        when s_while_check =>
          if DATA_RDATA = x"00" then
            nstate <= s_while_end;
          else
            nstate <= s_fetch;
          end if;
      
        when s_while_end =>
          mx1_select <= '0';
          pc_inc <= '1';
          nstate <= s_fetch;

        when s_write =>
          DATA_EN <= '1';
          DATA_RDWR <= '0'; 
          mx1_select <= '1';
          if OUT_BUSY = '0' then
            OUT_WE <= '1';
            OUT_DATA <= DATA_RDATA;
            nstate <= s_write2;
          elsif OUT_BUSY = '1' then
            nstate <= s_write;
          end if;
        
        when s_write2 =>
          mx1_select <= '0';
          pc_inc <= '1';
          nstate <= s_fetch;

        when s_halt =>
          pc_inc <= '0';
          pc_dec <= '0';
          READY <= '1';
          DONE <= '1';
          nstate <= s_halt;

        when s_others =>
          pc_inc <= '1';
          nstate <= s_fetch;

      end case;
  end process;

 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --      - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --      - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly. 

end behavioral;

