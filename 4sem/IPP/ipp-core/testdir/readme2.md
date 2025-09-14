### Implementačná dokumentácia k 2. úlohe do IPP 2024/2025
### meno a priezvisko: Juraj Mesík 
### Login: xmesikj00
___
### Architektúra 

Moje riešenie projektu zahŕňa vytvorenie dvoch hlavných typov tried:
- **Instace.php** definuje základnú triedu pre inštancie XML objektov
- **ClassInstance.php** definuje inštanciu užívateľom zadanej triedy
- **MethodInstance.php** inštancia metódy, dedí z `Instance`
- **AssignInstance.php** inštancia pre priradenie, dedí z `Instance`
- **ExpressionInstance.php** inštancia pre výraz
- **BlockInstance** inštancia bloku, dedí z `Instance` a vytvára vnútornú inštanciu bloku
- **ObjectCLass** definuje základnú triedu pre vytváranie konštrukcii jazyka SOL
- **BlockCLass** definuje triedu pre vnútornú reprezentáciu SOL bloku
- **StringClass** definuje triedu pre dátový typ `string` a obsahuje príslušné metódy
- **IntegerCLass** definuje triedu pre dátový typ `integer` a obsahuje príslušné metódy
- **NilCLass** definuje triedu pre vytváranie `Nil` objektov
- **TrueCLass** definuje triedu pre dátový typ `bool` s hodnotout `true`
- **FalseClass** definuje triedu pre dátový typ `bool` s hodnotout `false`
___
### Implementácia 

####  trieda `Instance`

Trieda obsahuje tri atribúty - `$type` určujúci o inštanciu čoho sa jedná, `$input` reťazec z štandardného vstupu a `$root`, ktorým je koreňový uzol danej inštancie v XML reprezentácii. Okrem toho deklaruje jednu metódu - `parseChildren()`, ktorá je implementovaná podľa typu inštancie. Na začiatku skriptu sa v `Interpreter.php` vytvára najvyšší typ inštancie - `ClassInstance()`, ktorá okrem typu, vstupu a koreňového uzla obsahuje aj atribút `$name` a `methodObjects[]`. Metódu `ParseChildren()` implementuje tak, že prechádza koreňový strom podľa názvu koreňa `method` a na základe selektoru vytvára `methodInstance` a pridá ju do atribútu `$methodObjects[]`. 

V `methodInstance` sa v jej jedinej metóde vytvorí inštancia bloku danej metody a block sa pridá do inštančného atribútu, ktorým je pole pre bloky.  

Inštancia `BlockInstance` vo funkcii `parseChildren()` vytvára inštanciu výrazu `ExpressionInstance` , ktorá je najnižšia užívateľom zadefinovaná štruktúra. Nad touto inštanciou zavolá metódu `value()`, ktorej výsledok vloží do zoznamu všetkým zadefinovaných premenných v súčasnom bloku.

Metóda `value()` je zodpovedná za vyhodnocovanie **čohokoľvek čo je pod štítkom `expr`**, ktorého DOMELement strom vždy dostane ako argument. Okrem toho tvoria jej argumenty aj názov premennej, v aktuálnom priradení a vstup užívateľa. Nasleduje rozklad jeho potomkov a nakoniec sa vytvorený objekt stáva návratovou hodnotou tejto metódy. Počas implementácie som vytvoril pomocnú funkciu `findLiteral()`, ktorá v prípade, že je podstrom definovaný ako literál, ktorý dedí z triedy, ktorá nie je medzi vstavanými triedami, dohľadá túto užívateľom definovanú triedu a cez rodičovské triedy sa rekurziou dostane až k hodnote názvu niektorej vstavanej triedy.

#### trieda `ObjectClass`

Táto trieda tvorí základ pre všetky vnútorné inštancie jazyka SOL a jej základnými atribútmi sú `$name` - meno daného objektu, `$type` - typ objektu, `$value` - hodnota akú objekt nadobúda. Obsahuje definíciu konštruktoru a deklarácie spoločných metód všetkých vnútorných inštancií. 

`BlockClass` definuje blok ako objekt, najmä kvôli možnosti priradenia bloku do premennej. Funguje veľmi podobne ako `BlockInstance`, ale je obohatený o atribúty `$argv[]`, ktorý slúži na predávanie konkrétnych hodnôt parametrov a `parameters[]`, ktorý je na ukladanie nájdených parametrov v definícií bloku. Jeho metóda `value()` má ako argument práve pole `$argv[]` a po jeho zadefinovaní nastavení zavolá metódu `parseChildren()`, čo spustí analýzu bloku podobne ako pri inštancií bloku. 


Predávanie všetkých parametrov (či už vstavaným metódam alebo blokom) je vyriešené cez jednoduché pole, ktoré sa v prípade, že sa jedna o parametrickú metódu naplní cez jednoduchý cyklus v metóde `value()`. Metódy, ktoré sa neskôr budú invokovať a ktoré obsahujú parametre, čerpajú hodnoty parametrov práve z poľa `$argv[]`. 


Nasleduje finálny cyklus, v ktorom prechádzam pole objektov a hľadám zhodu mena premennej, ktorej sa má poslať správa  s menom existujúceho objektu. Pri zhode sa podľa počtu parametrov správy invokuje metóda uložená v `$send_selector` nad objektom, ktorého meno `$name`sa zhoduje. Daná metóda väčšinou vracia nový objekt, takže celú invokáciu priradím do pomocnej premennej `$newObj`, ktorá je návratovou hodnotou metódy `value()` a je pridaná do poľa objektov.
Metóda `value()` obsahuje viacero rekurzívnych volaní hlavne kvôli jej shopnosti vyhodnocovať akýkoľvek `expr` strom, napr. pri definovaní hodnôt argumentov správy alebo pri vnorenom zasielaní správ. Metódy vnútorných objektov sú navrhnuté tak, aby vždy vracali nový objekt správneho typu a s aktualizovaným atribútom `value`, takže je rekurzívne volanie bezpečné - je iba potreba správne určiť koreňový strom.

### Diagram tried

![Class Diagram](./IPP-driagram.drawio.pdf)


### Zdroje
- https://github.com/Kubikuli/IPP_proj2-tests - testy

- https://moodle.vut.cz/mod/page/view.php?id=508609

- https://www.w3schools.com/php/