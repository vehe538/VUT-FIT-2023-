## Funkcionalita

### Preklad a spustenie

Preklad je realizovaný príkazom `make` v zdrojovom repozitári, ktorý preloží súbory v zložke `./src/` . Do zdrojovej zložky sa vytvorí binárny
súbor `./ipk25chat-client`, ktorý užívateľ spustí nasledovne: 

`./ipk25chat-client <ARGUMENTY>`

### Argumenty
```
- vypísanie nápovedy:                [ -h ]
- určenie typu protokolu pripojenia: [ -t tcp | -t udp ]                      (povinný argument)
- adresa cieľového servera:          [ -s <IPv4 adresa> | <hostname> ]        (povinný argument)
- cieľový port:                      [ -p <číselná hodnota veľkosti uint16> ] (predvolená hodnota je 4567)
- čas na potvrdenie UDP spojenia:    [ -r <počet ms> ]                        (predvolená hodnota je 250)
```


## Príklady spustenia

`./ipk25chat-client -t tcp -s localhost`

`./ipk25chat-client -t udp -s anton5.fit.vutbr.cz -r 5 -r 500`

## Limitácie
- program nepodporuje správanie klienta pri UDP komunikácii, len čiastočne je ošetrený stav `AUTH`. Do súboru [README.md](./README.md) som uviedol ako by som postupoval pri implementácii aj tejto varianty, ktorú som, žiaľbohu, nestihol dôkladne realizovať. Testy v [README.md](./README.md) obsahujú iba TCP komunikáciu.
- pri TCP komunikácii program nepodporuje `\n` v obsahu správy - takúto správu rozdelí, kde prvá časť je spracovaná, druhá vyhodnotená ako 'malformed message'. Okrem toho TCP funguje.





