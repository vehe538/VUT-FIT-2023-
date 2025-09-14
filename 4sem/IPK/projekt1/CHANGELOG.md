
## Funkcionalita
- určenie rozhrania: [-i interface | --interface interface]
- rozsah UDP portov: [ --pu <prvý>-<posledný> | -u <prvý>-<posledný> ]
- rozsah TCP portov: [ --pt <prvý>-<posledný> | -t <prvý>-<posledný> ]
- špecifikácia času na odpoveď: [ -w <počet ms>]    (nepovinný argument, predvolená hodnota je 5000)
- určenie adresy: [ hostname | ip-adresa zariadenia ]


 Pri ktoromkoľvek chýbajúcom povinnom argumente je zobrazený zoznam dostupných sieťových rozhraní. Chybové hlášky sú vypísované na štandardný chybový výstup `stderr` a ostatné výstupy na `stdout`. Preklad je možný príkazom `make`, ktorý vytvorí spustiteľný binárny súbor a a objektové súbory, následne `make clean` tieto súbory odstráni.

## Príklady použitia

`sudo ./ipk-l4-scan --interface wlp0s20f3 -u 15-20 --pt 440,443,80 -w 3000 147.229.2.90`

`sudo ./ipk-l4-scan -i lo --pu 120,43 www.youtube.com`

## Limitácie
- v programe nefunguje posielanie TCP packetov na IPv6 adresy
- pri zadávaní portov je možné zadať buď rozsah (`--pt 5-10`) alebo jednotlivé porty (`--pt 5,7,9,10`), ale nie ich kombináciu (`--pu 5-10,13`)
- v prípade, že sú argumenty zadané nesprávne (napr. `--p` alebo `-wait`), ale pritom  ostatné argumenty stačia na chod programu, program nevypíše error, ale chybný argument odignoruje, napr. `sudo ./ipk-l4-scan -i lo --pt 443 localhost --u 100-103 -wait 1000` zoskenuje iba TCP port 443




