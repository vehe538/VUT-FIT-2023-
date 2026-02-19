#include <simlib.h>

bool mistakesEnabled = true; // globalni prepinac pro zapnuti/vypnuti chybovosti na stanovistich

Facility Handler("Handler");
int tHandle = 360;

Facility Trash("Odpad");
int Trash_capacity = 100;
int Trash_count = 0;
int tTrash = 5;


// Forward declarations for classes used as pointer types before their
// full definitions appear later in this file.
class GeneratorTaktuS1;
class AutoExport;
class PrichodStanovistePosledni;
class StartLoadingTruck;

GeneratorTaktuS1 *generatorTaktuS1 = nullptr;
int pocetPracovnikuS1 = 1;
Store Stanoviste1("Stanoviste 1 - Takt", pocetPracovnikuS1);
int tTakt = 1*60; // takt pro stanoviste 1

int pocetPracovnikuS2 = 10;
Store Stanoviste2("Stanoviste 2 - Blistry", pocetPracovnikuS2);
Facility ItemsS2("Blistry pro Stanoviste 2"); // napr. blistry (kvetinace)
int ItemsS2_count = 50;
int ItemsS2_max = 50;
double SanceNaChybuS2 = 0.1; // 10% sance na chybu na stanovisti 2
double UspechZpracovaniS2 = 0.9; // 90%
int tZpracovaniS2 = 3.0*60.0;

int pocetPracovnikuS3 = 5;
Store Stanoviste3("Stanoviste 3 - Balicka", pocetPracovnikuS3);
Facility ItemsS3("Prepravni baleni pro Stanoviste 3"); // napr. prepravni baleni (krabice)
int ItemsS3_count = 20;
int ItemsS3_max = 20;
double SanceNaChybuS3 = 0.01; // 1% sance na chybu na stanovisti 3
double UspechZpracovaniS3 = 0.75; // 75%
int tZpracovaniS3 = 5.0*60.0;

Facility OdvozAutem("Odvoz autem");
int tPrijezduAutaExport = 3600;
int kapacitaAutaExport = 40;
int tNaklozeniRostlinyExport = 120;

Facility StanovistePosledniAuto("Stanoviste posledni auto");
int pocetRostlinNaStanovistiPosledni = 0;
AutoExport *autoExportNakladajici = nullptr;

Store StanovistePosledni("Stanoviste posledni", 1000); // kapacita prostoru na poslednim stanovisti


//Facility Picker("Zberac urody na plantazi");
int tFrekvenciaObjednavok = 2160;
int pocetZberacov = 1;
int tPozbieranieHriadky = 7200; // kolko Zberacovi trva pozbierat jednu hriadku
int pocetRastlinHriadka = 10; // pocet rastlin v jednej hriadke

Facility Driver("Vodic na trase plantaz-stanovisko1");
int plantCounter;
int tNalozenieRastlinyImport = 10; // nalozenie jednej rastliny
int kapacitaAutaImport = 15; // po nalozeni 60tich rastlin, driver odchadza
int tCasCesty = 2460;  // cas cesty pole - s1

 
Store PoleRastlin("Zrele rastliny na poli", 1000);
Store PozbieraneRastliny("pozbierane rastliny", 1000);
Store NavesKamionu("kufor auta", kapacitaAutaImport);
Store Zberaci("Zberaci na plantazi", pocetZberacov);


int stats_TotalPlantsPicked = 0;

int stats_TotalPlantsTaktedIn = 0;
int stats_TotalPlantsDefective = 0;
int stats_TotalPlantsExported = 0;
int stats_TotalPlantsPacked = 0;
int stats_TotalItemsUsed = 0;

int stats_TotalS1Processed = 0;
int stats_TotalS2Processed = 0;
int stats_TotalS3Processed = 0;

int stats_TimeToStartExport = 0;


class ProcessStanovisteGeneric : public Process {
    public: void Zpracovani(Facility& itemsFacility, int& itemsCount, int itemsMax, double tWork) {
        Seize(itemsFacility);

        if (itemsCount > 0) {
            Print("Zpracovávám položku. Zbývá položek: %d\n", itemsCount - 1);
            itemsCount--;
        } else {
            Print("Není dostatek položek, volám handlera pro doplnění...\n");
            // zavolej handlera pro doplnění
            Seize(Handler);
            Wait(Exponential(tHandle));
            itemsCount += itemsMax;
            Release(Handler);
        }
        Release(itemsFacility);
        Wait(Exponential(tWork));
        stats_TotalItemsUsed++;
    }

    public: void GoToTrash() {
        stats_TotalPlantsDefective++;

        Seize(Trash);

        if (Trash_count < Trash_capacity) {
            Print("Jdu do odpadu. Aktuální počet v odpadu: %d\n", Trash_count + 1);
            Trash_count++;
        } else {
            Print("Odpady jsou plné, volám handlera pro vyprázdnění...\n");
            // zavolej handlera pro vyprázdnění
            Seize(Handler);
            Wait(Exponential(tHandle));
            Trash_count = 1;
            Release(Handler);
        }
        Wait(Exponential(tTrash));
        Release(Trash);
    }
};
class AutoExport : public Process {

    int nalozenoRostlin = 0;

    void Behavior() {
        Seize(StanovistePosledniAuto);
        autoExportNakladajici = this;
        while (nalozenoRostlin < kapacitaAutaExport) {
            if (StanovistePosledni.Empty() /* pocetRostlinNaStanovistiPosledni == 0*/) {
                Print("Auto export: Cekam na rostliny...\n");
                Passivate();
                Print("Auto export: Probudil jsem se, pokračuji v nakládání...\n");
            }
            Wait(tNaklozeniRostlinyExport);
            nalozenoRostlin++;
            // pocetRostlinNaStanovistiPosledni--;
            Leave(StanovistePosledni, 1);
            
        }
        autoExportNakladajici = nullptr;
        Release(StanovistePosledniAuto);
        Print("Auto export: Auto je naloženo, odjíždím...\n");
        stats_TotalPlantsExported += nalozenoRostlin;
        if (stats_TimeToStartExport == 0) {
            stats_TimeToStartExport = (int) Time;
        }
    }
};


class PrichodStanovistePosledni : public Process {
    void Behavior() {
        stats_TotalPlantsPacked++;

        Print("PrichodStanovistePosledni: Rostlina přichází na poslední stanoviště...\n");
        bool probudAuto = false;
        if (pocetRostlinNaStanovistiPosledni == 0) {
            probudAuto = true;
        }

        Enter(StanovistePosledni, 1);
        // pocetRostlinNaStanovistiPosledni++;

        if (autoExportNakladajici != nullptr && probudAuto) {
            Print("PrichodStanovistePosledni: Probouzím auto export...\n");
            autoExportNakladajici->Activate();
        }
        
    }
};


class PrichodS3 : public ProcessStanovisteGeneric {
    void Behavior() {
        Enter(Stanoviste3, 1);
        Print("PrichodS3: Rostlina vstupuje do Stanoviste3...\n");
        
        if (mistakesEnabled){
            
            if (Uniform(0.0, 1.0) < SanceNaChybuS3) {
                // s pravděpodobností 1% jdi do odpadu
                GoToTrash(); // spatny "produkt" predchoziho stanoviste
                Leave(Stanoviste3, 1);
                return;
            }
        }
        
        bool povedlo_se = false;
        while (!povedlo_se) {
            Zpracovani(ItemsS3, ItemsS3_count, ItemsS3_max, tZpracovaniS3);
            if (Uniform(0.0, 1.0) < UspechZpracovaniS3) povedlo_se = true; // s pravděpodobností 90% se zpracování podařilo 
            if (!mistakesEnabled) povedlo_se = true;
        }
        Leave(Stanoviste3, 1);
        
        stats_TotalS3Processed++;
        (new PrichodStanovistePosledni())->Activate(); // nový příchod
    }
    
};


class PrichodS2 : public ProcessStanovisteGeneric {
    void Behavior() {
        Enter(Stanoviste2, 1);
        Print("PrichodS2: Rostlina vstupuje do Stanoviste2...\n");
        
        if (mistakesEnabled){
            if (Uniform(0.0, 1.0) < SanceNaChybuS2) {
                // s pravděpodobností 1% jdi do odpadu
                GoToTrash(); // spatna rostlina
                Leave(Stanoviste2, 1);
                return;
            }
        }

        bool povedlo_se = false;
        while (!povedlo_se) {
            Zpracovani(ItemsS2, ItemsS2_count, ItemsS2_max, tZpracovaniS2);
            if (Uniform(0.0, 1.0) < UspechZpracovaniS2) povedlo_se = true; // s pravděpodobností 90% se zpracování podařilo 
            if (!mistakesEnabled) povedlo_se = true;
        }
        Leave(Stanoviste2, 1);
        
        stats_TotalS2Processed++;
        (new PrichodS3())->Activate(); // nový příchod
    }
    
};


class GeneratorTaktuS1: public Process {
    void Behavior() {

        // kdyby bylo vice paralelnich linek, tak je treba semafor
        while (true){
            // Print("GeneratorTaktuS1: Generuji takt pro Stanoviste1... qlen %d\n", Stanoviste1.QueueLen());
            if(Stanoviste1.Empty()) {
                // Print("GeneratorTaktuS1: Fronta je prázdná, nepřidávám další proces.\n");
                this->Passivate();
                // Print("GeneratorTaktuS1: Proces probuzen.\n");
            }
            
            Leave(Stanoviste1, 1);
            
            stats_TotalPlantsTaktedIn++;
            (new PrichodS2())->Activate();

    
            
            Wait(tTakt); 

        }
    }
};

class VylozRostlinyNaStanoviste1 : public Process {
    void Behavior() {
        Print("VylozRostlinyNaStanoviste1: Vykládám rostliny na Stanoviste1...\n");
        
        if (generatorTaktuS1 == nullptr) {
            
            generatorTaktuS1 = new GeneratorTaktuS1();
            generatorTaktuS1->Activate();

        } else if (generatorTaktuS1->Idle())
        {
            Print("VylozRostlinyNaStanoviste1: Probouzím generatorTaktuS1...\n");
            generatorTaktuS1->Activate();
        }
        
    }
};


class GenAutoExport : public Event {
    void Behavior() {
        Print("GenAutoExport: Generuji auto export...\n");
        (new AutoExport())->Activate();
        Activate(Time + tPrijezduAutaExport);
    }
};


class VylozRastlinu : public Process {

    void Behavior() {
        Seize(Driver, 2);
        Leave(NavesKamionu, 1);
        Wait(Exponential(tNalozenieRastlinyImport));
        Enter(Stanoviste1, 1);
        (new VylozRostlinyNaStanoviste1())->Activate();
        Release(Driver);
    }

};

class OdchodKamionu: public Process {

    void Behavior() {

        Print("Driver odchadza s plnym kufrom rastlin na S1...\n");
        Seize(Driver, 1);
        Wait(Exponential(tCasCesty));
        Release(Driver);

        Print("Driver dosiel na S1 a vyklada rastliny\n"); 
        for (int i = 0; i < kapacitaAutaImport; i++)
        {
            (new VylozRastlinu())->Activate();  
        }
        
    }
};



class NalozRastlinu : public Process {

    void Behavior() {

        Seize(Driver);
        Leave(PozbieraneRastliny, 1);
        Wait(Exponential(tNalozenieRastlinyImport));
        Enter(NavesKamionu, 1);
        Release(Driver);

        if (NavesKamionu.Full()) {

            (new OdchodKamionu())->Activate();
            plantCounter = 1;
        
        } else {
            if (!PozbieraneRastliny.Empty()) {

                (new NalozRastlinu()) ->Activate();
            } else {
                Print("Driver caka na plantazi, nema plny naves...\n");
            }
        }
    }

};

class ZbieranieRastlin : public Process {

    void Behavior() {
        
        Enter(Zberaci, 1); // ekvivalent Seize jedneho zberaca
        for (int i = 0; i < pocetRastlinHriadka; i++) {
            // Leave(PoleRastlin, 1);
            Enter(PozbieraneRastliny, 1);
            stats_TotalPlantsPicked++;
        }
        Wait(tPozbieranieHriadky);
        Leave(Zberaci, 1); // ekvivalent Release jedneho zberaca

        

        (new NalozRastlinu())->Activate();

    }
};

class GeneratorPlantaz : public Process {

    void Behavior() {
        
        ZbieranieRastlin *startPicking = new ZbieranieRastlin();
        startPicking->Activate();

        (new GeneratorPlantaz())->Activate(Time + tFrekvenciaObjednavok);
            
    }
};




/**
 * Tady budem nastavovat vsechny parametry simulace pro ruzne experimenty
 * 
 * napr. muzem udelat jednu funkci pro nastaveni kazdeho experimentu
 */
void setOptionsDefault() {
    mistakesEnabled = true;


    // VVVVVVVV Handler
    tHandle = 5*60; // cas obsluhy handlera

    // VVVVVVVV Odpad
    Trash_capacity = 100; // kapacita odpadu
    tTrash = 2*60; // cas vyhozeni do odpadu

    // VVVVVVVV Pole
    tFrekvenciaObjednavok = 2160;
    tPozbieranieHriadky = 7200;
    pocetRastlinHriadka = 10;
    pocetZberacov = 1;

    // VVVVVVVV Driver + Cesta na S1
    tNalozenieRastlinyImport = 10;
    kapacitaAutaImport = 15;
    tCasCesty = 2460;

    // VVVVVVVV Stanoviste 1 Takt
    pocetPracovnikuS1 = 1;
    tTakt = (1*60)/10; // takt pro stanoviste 1 (aby stihalo stanoviste 2)

    // VVVVVVVV Stanoviste 2 Blistry
    pocetPracovnikuS2 = 15;
    ItemsS2_max = 150;
    SanceNaChybuS2 = 0.02; // 2% sance na chybu na stanovisti 2
    UspechZpracovaniS2 = 0.9; // 90%
    tZpracovaniS2 = 5.0*60.0;

    // VVVVVVVV Stanoviste 3 Balicka
    pocetPracovnikuS3 = 10;
    ItemsS3_max = 100;
    SanceNaChybuS3 = 0.01; // 1% sance na chybu na stanovisti 3
    UspechZpracovaniS3 = 0.90; // 90%
    tZpracovaniS3 = 3.0*60.0;

    // VVVVVVVV Auto Export
    tPrijezduAutaExport = 60*60*1; // perioda příjezdu aut
    kapacitaAutaExport = 60;
    tNaklozeniRostlinyExport = 120; // cas nalozeni 1 rostliny ("opatrne naklada")
}
    

int main() {
    // SetOutput("main_output.txt");
    setOptionsDefault();
    
    
    Init(0.0, 60*60*24*10);
    

    // Inicializace poctu polozek na stanovistich
    ItemsS2_count = ItemsS2_max;
    ItemsS3_count = ItemsS3_max;

    // Spustit generátor příchodů na plantáž
    (new GeneratorPlantaz())->Activate();

    // Spustit generátor aut pro odvoz
    (new GenAutoExport)->Activate(3600);

    Run();
    
    Zberaci.Output();
    PoleRastlin.Output();
    PozbieraneRastliny.Output();
    Driver.Output();
    Stanoviste1.Output();
    Stanoviste2.Output();
    Stanoviste3.Output();
    // StanovistePosledniAuto.Output();
    StanovistePosledni.Output();
    
    Handler.Output();
    
        Print("\n\nTotal plants picked: %d\n", stats_TotalPlantsPicked);
        Print("Plants arrived to S1: %d\n", stats_TotalPlantsTaktedIn);
        Print("Total plants defective: %d\n", stats_TotalPlantsDefective);
        Print("Total items used: %d\n", stats_TotalItemsUsed);
        Print("Total S2 processed: %d\n", stats_TotalS2Processed);
        Print("Total S3 processed: %d\n", stats_TotalS3Processed);
        Print("Total plants packed: %d\n", stats_TotalPlantsPacked);
        Print("Total plants exported: %d\n", stats_TotalPlantsExported);

        Print("Time to start first export: %d seconds\n", stats_TimeToStartExport);

    return 0;
}



