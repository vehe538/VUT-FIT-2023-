
# Dokumentácia k projektu IPP
## Popis návrhu riešenia
Každému prvku návrhu je ako jeden z argumentov predaný príslušný XML koreň, ktorý prvok podľa svojej funkcie prechádza a predáva svoje podkorene ďalším prvkom. Okrem toho sa prvkom predáva **zásobník rámcov** pre bloky, aktuálny **triedny rámec** a **programový rámec** (podrobnejšie vysvetlenie ich významu v časti [Dátové štruktúry](#dátové-štruktúry)).
Na začiatku, pre vytvorenie [základných štruktúr](#ako-základné-štruktúry-budem-v-texte-uvažovať-xml-koreň-zásobník-rámcov-pre-bloky-triedny-rámec-a-programový-rámec)*, program iteruje cez základný koreň `program` a pre každú triedu vytvára triedny rámec a s ním potom  tzv. *triedny záznam*. Taktiež kontroluje prítomnosť triedy `Main` a jej metódy `run`. Po tomto cykle sa nad triedou `Main` zavolá metóda `run`, ktorá pošle objektu bloku, ktorý jej patrí správu `value`, čím začína samotná interpretácia. Program by sa teda dal rozdeliť do dvoch väčších častí:

###### * *ako základné štruktúry budem v texte uvažovať XML koreň, zásobník rámcov pre bloky, triedny rámec a programový rámec*
### 1. **sémantická kontrola**, vytvorenie triednych rámcov, triednych záznamov a zásobníka rámcov

   Počas iterovania cez XML korene tried sa pre každú triedu vytvára **triedny rámec** a **triedny záznam**. Triedny rámec slúži na ukladanie selektorov metód a ich blokov - ako jediný atribút obsahuje slovník, v ktorom **kľúčami sú selektory** (identifikátory metód) a **hodnotami sú k ním korešpondujúce bloky** ( inštancie triedy `ObjectBuiltin`, ktorá reprezenuje blok v SOL26). Triedny záznam je štruktúra, ktorá umožňuje fungovanie SOL26 programov s *dlhšou hierarchiou dedičnosti*. Hlavný rozdiel medzi triednym záznamom a triednym rámcom je, že triedny rámec je viazaný na *jednu konkrétnu triedu a iba jej metódy* a triedny záznam umožňuje *dedičnosť naprieč triedami*. Triedny rámec je jeden z atribútov triedneho zaznamu. Trieda pre triedny záznam obsahuje súkromné metódy na vyhľadávanie rodičovského záznamu alebo základného objektu pre správne vytváranie inštancií a verejnú metódu `methods_extension()`, ktorá pri vytvorení novej inštancie v SOL26 programe *umožní volať zdedené metódy*. Grafické znázornenie rozdielu na jednoduchom príklade:

   ![class_record vs. class_frame](docs-picture1.png) 

   Na obrázku sú v červenom obdĺžniku všetky časti kódu, pre ktoré sú vytvorené **triedne rámce** a v tyrkysovom obdĺžniku časť kódu, pre ktorý je vytvorený **triedny záznam a jeho atribúty pre triedu B**. Pre triedy `Main` a `A` by sa označenia triednych rámcov a triednych záznamov nelíšili ( tyrkysový obdĺžnik by ohraničoval tú istú oblasť ), no pre triedu `B` zaisťuje práve `methods_extension()` prístup k metódam rodiča (`helloA`). Ďalej metóda `_parent_lookup()` nastaví atribút záznamu `parent_class` na `A` a metóda `_base_lookup()` nastaví atribút `base_obj` na `Object`. Takto zaistíme, že inštancia vytvorená v `Main` vie, *ktorý zo vstavaných objektov reprezentuje, ktorú `run` metódu má zavolať a ktorým správam rozumie z hľadiska dedičnosti*. Keďže sa triedne záznamy vytvárajú pred volaním `run` v `Main` triede, je tiež pomerne jednoduché odchytiť viaceré statické sémantické chyby. ( konkrétne kontroly sú uvedené v časti [Dátové štruktúry](#triedny-záznam-classrecord) )

### **2. interpretácia**, pracovanie so zásobníkom rámcov, vytváranie štruktúr pre vyhodnocovanie výrazov
   Po zavolaní `value()` nad blokom, ktorý je uložený v triednom rámci pri `run` selektore sa vytvorí nový **blokový rámec**, ktorý sa pridá na vrch zásobníka rámcov. Následne program iteruje cez pole priradení `assign_arr[]`( ktoré bolo naplnené vďaka metóde `_handle_assings()` pri vytvorení objektu bloku ) a nad každým priradením zavolá metódu `exec()`. Priradenie `Assign` som implementoval ako triedu, ktorá má okrem [základných atribútov](#ako-základné-štruktúry-budem-v-texte-uvažovať-xml-koreň-zásobník-rámcov-pre-bloky-triedny-rámec-a-programový-rámec)* aj atribút `name` ( ľavá strana priradenia ) a `expr` ( práva strana priradenia ), ktoré sa vyhodnotia podľa XML koreňa. Atribút `expr` môže mať 4 typy: **literál, premenná, zaslanie správy alebo blokový literál**. Každý typ má implementovanú metódu `get_value`, ktorá vždy vráti niektorý zo vstavaných objektov. V metóde `exec()` sa referencia na získaný objekt zapíše do blokového rámca ku menu premennej ( atribút `name`) a referencia na získaný objekt je zároveň návratovou hodnotou funkcie `exec()`. Po vyhodnotení všetkých priradení zo zoznamu `assign_arr[]`, je blokový rámec zo zásobníka odobraný. Ak je pole prázdne, metóda `value()` vráti `Nil`. Inštancia objektu, ktorý vráti metóda `exec()` je priebežne ukladaná do pomocnej premennej `last`, ktorá je potom návratovou hodnotou `value()` metódy.\
   Metóda `get_value()` je implementovaná rôzne v závislosti na type výrazu:\
   V prípade literálu sa vytvorí objekt podľa atribútu XML koreňa `class`. V prípade vytvárania podľa užívateľom definovanej triedy sa objekt určí práve vďaka hodnote `base_obj` **daného triedneho záznamu** v programovom rámci.\
   Ak je výrazom premenná, vyhľadáva sa v aktuálnom blokovom rámci, pričom je ošetrené správne zaobchádzanie zo `self` a `super` podľa nepovinného argumentu `is_receiver: bool`. Inak `get_value()` vráti referenciu na objekt podľa mena premennej z rámca na vrchu zásobníka.\
   V prípade, že je výrazom zaslanie správy, `get_value()` najprv **vytvorí kópiu prijímateľa**, aby mohla byť jej hodnota zmenená metódou, a podľa selektora postupuje v tomto poradí:
   1. skontroluje či sa nejedná o **vstavanú metódu** podľa typu prijímateľa a prípadne zavolá danú metódu
   2. skontroluje, či prijímateľ nemá **atribút** ktorého meno sa zhoduje so selektorom ( pre **prípad čítania atribútu** ) a prípadne vráti referenciu na tento atribút
   3. skontroluje, či sa selektor nachádza **medzi zdedenými metódami** ( prípadne získané pomocou `methods_extension()` )
   4. skontroluje sa aktuálny **triedny záznam** a prípadne pošle správu `value()` príslušnému bloku 
   5. ak je to možné a správa má práve jeden parameter, vytvára pre prijímateľa **nový atribút** s názvom selektora
   6. vyvolá chybu typu DNU

   Toto správanie je implementované pomocou `try: except:` konštrukcii a taktiež sa v tejto časti vyskytujú viaceré **dynamické sémantické kontroly** (32,33) a kontroly na **behové chyby interpretácie**(51-54).\
   V prípade bloku vracia `get_value()` samotnú inštanciu (self) - do rámca sa zapíše iba referencia na blok.

## UML
Diagram tried som rozdelil na dve časti ( podľa popisu návrhu ). Triedy a štruktúry, ktoré boli z väčšej časti súčasťou šablóny sú označené červenou farbou.
Diagramy sa nachádzajú v separátnych súboroch kvôli prehľadnosti.

## Dátové štruktúry
 - ### Rámec `Frame`
   Slúži na ukladanie referencií objektov podľa selektoru / mena premennej / mena triedy. Obsahuje metódu na zápis, čítanie a vypísanie prvkov rámca. Referencie sa ukladajú do atribútu `obj_dict{}`. Má troch potomkov:
   1. Programový rámec `ProgramFrame` - v slovníku `obj_dict` je kľúčom reťazec **identifikátoru triedy** a hodnotou je referencia na **triedny záznam** ( inštancia `ClassRecord` )
   2. Triedny rámec `ClassFrame` - v slovníku je kľúčom reťazec **selektoru metódy** a hodnotou referencia na štruktúru **bloku** ( inštancia `BlockBuiltin` )
   3. Blokový rámec `BlockFrame` - v slovníku je kľúčom reťazec s názvom premennej a hodnotou je referencia na niektorý **vstavaný objekt alebo výraz typu blok**  ( inštancia niektorého z potomkov triedy `Expression` )
    ---

 - ### Triedny záznam `ClassRecord`
   Slúži na ukladanie informácií dedičnosti SOL26 tried. Atribúty sú okrem [základných štruktúr](#ako-základné-štruktúry-budem-v-texte-uvažovať-xml-koreň-zásobník-rámcov-pre-bloky-triedny-rámec-a-programový-rámec) pole názvov vstavaných tried, základný objekt `base_obj: str` a rodičovská trieda `parent_class: str`. Popis metód:
   - `_base_lookup()` rekurzívne prechádza všetky rodičovské triedy až kým nenarazí na rodiča, ktorý je jeden zo vstavaných objektov - toho uloží do atribútu `base_obj`. Obsahuje sémantickú kontrolu pre kruhovú dedičnosť (35).
   - `_parent_lookup()` v programovom rámci hľadá triedny záznam, ktorej identifikátor je zhodný s názvom atribútu `parent` aktuálneho XML koreňa. V prípade neúspechu je tu ošetrená situácia nedefinovanej triedy (32).
   - `_parse_methods()` prechádza podkorene aktuálneho XML koreňa a vytvára inštancie objektu `BlockBuiltin` ( spomenutý nižšie ), ktoré **zapisuje do svojho triedneho rámca** ako hodnoty ku kľúčom, ktoré tvoria názvy selektorov -  *v tejto časti sa pre každú triedu plní jej triedny rámec*. Obsahuje sémantickú kontrolu zhody arity selektoru s príslušným koreňom bloku (33) a tiež kontrolu redefinície metódy (35).
   - `methods_extension()` využíva sa v prípade, že je nejaký literál vytvorený z iného, než vstavaného objektu, a teda je potrebné *nastaviť, ktorým všetkým správam bude nový SOL26 objekt rozumieť*. Metóda rekurzívne prechádza programový rámec a za využitia atribútu `parent_class` postupne skladá nový slovník, kde kľúčami sú selektory metód a hodnotami sú k ním prislúchajúce bloky ( podobne ako pri triednom rámci ). Výsledkom je slovník, ktorý je novej inštancií **dynamicky** nastavený ako **interný atribút `methods`**, ktorý sa neskôr kontroluje pri type výrazu zasielania správy.

    ---
 - ### Výraz `Expression`
   Tvorí rozhranie pre typy výrazov `ExprVariable` , `ExprLiteral` a `ExprMessage`. Každý z potomkov obsahuje vlastnú implementáciu metódy `get_value()`. Trieda slúži na vyhodnocovanie pravej strany príkazu priradenia.
    
    ---
 - ### Priradenie `Assign`
   Trieda reprezentuje jeden celý príkaz priradenia, pričom ľavá strana je interný atribút `name` a práva strana je jedným z potomkov triedy `Expression` alebo objekt `BlockExpr`.  Obsahuje metódu `exec()`, ktorá na aktuálny blokový rámec zapíše ku kľúču z atribútu `name` výsledný objekt metódy výrazov `get_value()`.

    ---
 - ### Blok `Block`
   Tvorí rozhranie pre štruktúry, ktoré reprezentujú bloky. Každý z potomkov implementuje súkromné metódy `_handle_assigns()` a `_handle_args()` a verejnú metódu `value()` a vstavanú metódu `whileTrue`. 
   Súkromné metódy:
   - `_handle_assigns()` prechádza XML koreň a ku každému prvku so značkou `assign` vytvorí nový inštanciu triedy `Assign`, ktorú uloží do svojho interného atribútu `assign_arr[Assign]`. Obsahuje statickú sémantickú kontrolu na vpisovanie do formálneho parametru bloku (34).
   - `_handle_args()` prechádza XML koreň a ku každému prvku so značkou `parameter` vytvorí pole o dvoch prvkoch, kde prvým je meno parametra ( atribút XML koreňa `name` ) a druhým je poradie ( atribút XML koreňa `order`). Toto pole sa nakonci pridá do atribútu  `args[ [str, int] ]`. Metóda obsahuje statickú sémantickú kontrolu pre kolízie mien parametrov bloku (35).

   Potomkovia sú:
   1. `BlockBuiltin` označuje triedne metódy ( blok, ktorý sa v SOL26 nachádza v definícií triedy hneď za nejakým selektorom )
   2. `ExprBlock` označuje druh výrazu na pravej strane priradenia
   3. `EmptyBlock` používa sa v prípade, že je do premennej priradený literál typu blok ( koreňom je `<literal class="class" value="Block">` )

    ---
 - ### SOL26 Objekt `Object`
   Tvorí vnútornú reprezentáciu vstavaného základného objektu SOL26. Má vnútorný atribút `obj_value`, ktorý ukladá hodnotu objektu a obsahuje definície vstavaných metód podľa zadania. Dedia z neho ostatné vstavané triedy `String`, `Integer`, `TrueC`, `FalseC`, `NilC` a  `Block`. Každý potomok upravuje správanie vstavaných metód podľa špecifikácie SOL26.

## Návrhové vzory
 - ### Wrapper
   Funkciu tohto návrhového vzoru spĺňa trieda `Result`, ktorú používam ako *typ návratovej hodnoty niektorých vstavaných metód* v prípadoch, kde som usúdil, že bude prehľadnejšie vrátiť konkrétnu výslednú hodnotu namiesto nového objektu z novou hodnotou.\
   Obsahuje atribút `value: Any`, do ktorého je zapísaná hodnota výsledku nejakej metódy a  podľa ktorého je neskôr vytvorený správny objekt. Trieda, ktorá reprezenuje SOL26 vstavaný objekt `Object` a jeho potomkovia obsahujú atribút `result` typu `Result`, ktorý je vytvorený pri inicializácii.
   `Result` tiež obsahuje atribút `err: int`, ktorý v prípade chyby nadobudne niektorý z chybových kódov a `err_msg: str`, ktorý obsahuje správu súvisiacu s nájdenou chybou. Tieto dva atribúty sa neskôr využijú pri vytvorení `InterpreterError` hlásenia.

## Riešené problémy
 - ### Použitie šablóny
   V projekte som sa rozhodol nepoužiť časť šablóny, a to konkr. `input_model.py` a tým pádom prechádzať XML strom manuálne so zámienkou, že budem mať nad spracovaním XML stromu väčšiu kontrolu a že budem prehľadnejšie spracovávať poradie XML prvkov a vnorené korene.\
   Na konci implementácie interpretu  som si všimol, že môj prístup mal, aj napriek tomu, že mi program zväčša fungoval, viac nevýhod ako výhod a že veľa problémov, na ktoré som narazil by vyriešil za mňa práve `input_model.py` ( napr. jednoduchšie získavanie atribútov XML koreňov, nemusel by som prvkom zbytočne predávať podkorene alebo manuálne kontrolovať značky XML koreňov).  V prípade, že by vstup nebol XML, ale iný formát ( napr. `JSON` ), by som v mojom programe musel urobiť viac úprav, než za použitia `input_model.py` modulu, kedy by stačilo spraviť pár menších úprav na začiatku programu. Keby projekt robím odznovu, `input_model.py` určite použijem.

## Úpravy vo vylepšenej verzii
 - ### Pridanie statických sémantických kontrol ( Interpret )
   Statické sémantické kontroly mi v predošlom riešení chýbali, a teda som ich pridal do definícii metód triedy pre triedny záznam `ClassRecord`. Ide hlavne o chybové kódy 32, 33 a  35.
 - ### Odstránenie jedináčka pre SOL26 triedu `Main` ( Interpret )
   V pôvodnej verzii riešenia som mal pre SOL26 triedu `Main` vytvorenú samostanú triedu `ClassMain` , ktorej inštanciu som vytváral, pri vytváraní triednych záznamov ( okrem triedneho záznamu `Main` existoval aj jedináčik triedy `ClassMain` ). Trieda obsahovala iba kontrolu existencie `run` metódy a volala nad príslušným blokom `value()`. Toto som vedel jednoducho vyriešit prístupom do programového rámca `Main`, a teda trieda `ClassMain` bola zbytočná.
 - ### Odstránenie rozhrania pre `Assign`  ( Interpret ) 
   Všimol som si, že som v skoršej fáze riešenia projektu vytvoril rozhranie `Assign`, s ktorým som mal širšie úmysly, no nakoniec z neho dedila iba trieda `AssignXml` ( ktorá je v súčasnom riešení pomenovaná iba `Assign` ) a nič iné. Toto rozhranie nakoniec nemalo žiadny hlbší význam a bolo v programe zbytočne.
 - ### Nastavenie cesty na zapisovanie dočasných súborov ( Testovací nástroj )
   V testovacom nástroji sa v pôvodnom riešení zapisovali dočasné súbory ( ako napr. štandardný výstup interpretu, ktorý sa kontroloval s očakávaným výstupom ) do zložky s testami, ktorá bola počas hodnotenia pripojená v režime `read-only`, a teda sa nič nezapísalo kvôli právam. Vo vylepšenej verzii som na začiatok programu pridal vytvorenie a na konci vymazanie dočasného adresára na zápis pomocou `mkdtempSync` a `tmpdir()` a adekvátne upravil cesty pri vytváraní dočasných súborov. 
  
## Rozšírenia návrhu
   ### 1. Podpora rozhraní
   Reprezentáciu SOL26 rozhrania by som implementoval ako triedu `ClassInterface`, ktorá by ako atribút obsahovala **zoznam požadovaných selektorov**. Triede `ClassRecord` by som pridal nový atribút, ktorým by bol **zoznam identifikátorov rozhraní `ClassInterface`** a existujúca  metóda triedneho záznamu `_parse_methods()` by po novom pomocou  `methods_extension()` ( ktorá hľadá zdedené metódy ) kontrolovala, či sa v selektory metód aktuálnej triedy a selektory  zdedených metód zhodujú s tými, ktoré sú uložené v atribúte príslušného rozhrania `ClassInterface`.\
   Všetky rozhrania by mohli byť uložené pod svojím identifikátorom v **programovom rámci**,  do ktorého už má v existujúcom riešení triedny záznam prístup, pričom by som pri prístupe do programového rámca pridal jednoduchú kontrolu typu hodnoty ( teda, či sa jedná o triedny záznam alebo rozhranie ) pomocou `isinstance()`.
   ### 2. Modifikátory prístupu
   V súčasnom riešení sú metódy uložené v triednych rámcoch ako dvojice kľúč-hodnota, kde kľúčom je selektor a hodnotou je referencia na inštanciu triedy `BlockBuiltin`. Pre dosiahnutie podpory modifikátorov prístupu by som *zmenil spôsob ukladania do triedneho rámca* tak, že by sa ku kľúču **ukladala dvojica** ( zoznam alebo pole ), kde prvá položka by bol **modifikátor prístupu** ( jeden z reťazcov `public`,`private` alebo `protected` ) a druhá by bola samotná **referencia na inštanciu triedy** `BlockBuiltin`.
   Modifikátory by sa kontrolovali pri zasielaní správy v existujúcej  metóde `_send_message()` triedy `ExprMessage`, ktorá by skontrolovala **modifikátor a kto je príjemcom správy** a v prípade nedovolenej kombinácie, vyvolala chybu 51.

## Využitie AI
AI som využíval so na:
 - vysvetlenie **návrhových vzorov**, OOP terminológie a **dobrých zvyklostí**
 - otázky ohľadom **syntaxe** použitých jazykov ( python, TS )
 - **vysvetlenie chybových hlášok** implementačných jazykov
 - **generovanie `.test` súborov** + overenie očakávaných výstupov
 - vysvetlenie častí špecifikácie a odôvodnenie očakávaných výstupov ( **čo** by mal  SOL26 program vrátiť, **prečo** a **kde je to v špecifikácií spomenuté** )
 - určenie a zdôvodnenie **chybových kódov** podľa testovacieho scenára
 - v dokumentácii **kontrola gramatiky, konzistencie a správneho použitia OOP pojmov**
 
 Odkazy na konkrétne konverzácie sa nachádzajú v samostatnom súbore `ai-claude.md`.
### Ostatné zdroje
[https://www.gleek.io/blog/class-diagram-arrows](https://www.gleek.io/blog/class-diagram-arrows)

[https://diakritika.brm.sk/](https://diakritika.brm.sk/)

[https://www.markdownguide.org/cheat-sheet/](https://www.markdownguide.org/cheat-sheet/)