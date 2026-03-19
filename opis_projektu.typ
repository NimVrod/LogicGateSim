#set document(
  title: "LogicGateSim – Dokumentacja Projektu",
  author: "PwJCpp",
)

#set page(
  paper: "a4",
  margin: (top: 2.5cm, bottom: 2.5cm, left: 2.5cm, right: 2.5cm),
  numbering: "1",
  number-align: center,
)

#set text(
  font: "New Computer Modern",
  size: 11pt,
  lang: "pl",
)

#set heading(numbering: "1.1.")

#set par(justify: true, leading: 0.65em)

// ─────────────────────────────────────────────────────────────────────────────
//  STRONA TYTUŁOWA
// ─────────────────────────────────────────────────────────────────────────────
#align(center)[
  #v(3cm)
  #text(size: 28pt, weight: "bold")[LogicGateSim]
  #v(0.5cm)
  #text(size: 16pt, style: "italic")[Wizualny symulator układów logicznych]
  #v(0.5cm)
  #line(length: 60%, stroke: 0.5pt)
  #v(0.5cm)
  #text(size: 13pt)[Dokumentacja projektu]
  #v(1cm)
  #text(size: 11pt, fill: gray)[Projekt zaliczeniowy — PwJCpp \ #datetime.today().display("[day].[month].[year]")]
  #v(3cm)
]

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  SPIS TREŚCI
// ─────────────────────────────────────────────────────────────────────────────
#outline(
  title: "Spis treści",
  indent: 1.5em,
)

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  1. WPROWADZENIE
// ─────────────────────────────────────────────────────────────────────────────
= Wprowadzenie

*LogicGateSim* jest desktopowym, graficznym symulatorem układów logicznych zrealizowanym w języku C++23. Inspiracją do jego powstania był profesjonalny program *Multisim* firmy National Instruments – narzędzie powszechnie używane w dydaktyce elektroniki. LogicGateSim celuje w tę samą niszę: umożliwia projektowanie, łączenie i symulowanie kombinacyjnych oraz sekwencyjnych układów cyfrowych bezpośrednio na ekranie komputera, oferując przy tym interaktywne środowisko z niskim progiem wejścia.

Projekt powstał na potrzeby przedmiotu *Programowanie w Języku C++* i prezentuje zaawansowane techniki nowoczesnego C++: zarządzanie zasobami przez inteligentne wskaźniki, wzorce projektowe (Factory, Singleton), serializację opartą na XML oraz rendering 2D za pomocą SFML z nakładką ImGui.

== Wymagania systemowe i stos technologiczny

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Komponent*], [*Szczegóły*],
  [Język], [C++23],
  [Kompilatory], [MSVC 2022 / GCC 13+ / Clang 17+],
  [System budowania], [CMake 4.1.1+],
  [Rendering 2D], [SFML (Simple and Fast Multimedia Library)],
  [Interfejs użytkownika], [Dear ImGui],
  [Okna dialogowe plików], [Native File Dialog Extended (nfd)],
  [Serializacja], [TinyXML-2],
  [Zasoby], [Wewnętrzny ResourceManager (fonty, tekstury)],
)

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  2. ARCHITEKTURA PROJEKTU
// ─────────────────────────────────────────────────────────────────────────────
= Architektura projektu

Kod źródłowy podzielony jest na cztery główne moduły:

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Moduł*], [*Odpowiedzialność*],
  [`Core/`], [Model domenowy: komponenty, piny, bramki, przerzutniki],
  [`Simulation/`], [Silnik symulacji: klasa `Circuit` zarządzająca stanem układu],
  [`Serialization/`], [Zapis i odczyt układów w formacie XML],
  [`UI/`], [Warstwa prezentacji: ImGui + SFML, obsługa zdarzeń myszy/klawiatury],
)

Zależności przepływają od `Core` → `Simulation` → `Serialization`/`UI`, co zapewnia separację logiki od prezentacji.

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  3. KOMPONENTY
// ─────────────────────────────────────────────────────────────────────────────
= Komponenty logiczne

== Zarys hierarchii klas

Wszystkie elementy układu dziedziczą po abstrakcyjnej klasie `Component`. Każdy komponent posiada:
- kolekcję *pinów wejściowych* (`inputs`) i *pinów wyjściowych* (`outputs`),
- metodę `calculate()` aktualizującą wartości wyjść na podstawie wejść,
- metodę `draw()` odpowiedzialną za renderowanie graficzne,
- unikalny identyfikator numeryczny (`id`) i etykietę tekstową.

Piny (klasa `Pin`) przechowują aktualny stan logiczny (0 lub 1) oraz mogą być połączone drutami (`Wire`).

== Bramki logiczne

Bramki są podstawowymi elementami kombinacyjnymi. Każda bramka przyjmuje *od 2 do 8 wejść* (konfigurowalnych przez suwak w interfejsie), z wyjątkiem bramki NOT, która zawsze ma jedno wejście.

#table(
  columns: (auto, auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Klasa*], [*Nazwa*], [*Opis działania*],
  [`AndGate`], [AND], [Wyjście HIGH tylko gdy *wszystkie* wejścia są HIGH (iloczyn logiczny)],
  [`OrGate`], [OR], [Wyjście HIGH gdy *co najmniej jedno* wejście jest HIGH (suma logiczna)],
  [`NotGate`], [NOT], [Negacja – odwraca stan pojedynczego wejścia],
  [`NandGate`], [NAND], [Negacja iloczynu AND; wyjście LOW tylko gdy wszystkie wejścia HIGH],
  [`NorGate`], [NOR], [Negacja sumy OR; wyjście HIGH tylko gdy wszystkie wejścia LOW],
  [`XorGate`], [XOR], [Wyjście HIGH gdy *nieparzysta* liczba wejść jest HIGH],
  [`XnorGate`], [XNOR], [Negacja XOR; wyjście HIGH gdy parzysta liczba wejść HIGH],
)

== Komponenty wejściowe

Elementy pozwalające użytkownikowi wprowadzać sygnały do układu:

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Komponent*], [*Opis*],
  [*Przycisk* (`Button`)],
    [Interaktywny przycisk – kliknięcie lewym przyciskiem myszy toggleuje stan (HIGH/LOW). Użyteczny do ręcznego podawania sygnałów testowych.],
  [*Wejście* (`InputComponent` / „Input Pin")],
    [Symboliczny pin wejściowy układu. Gdy bieżący układ jest zapisywany jako komponent własny, każde `InputComponent` staje się jednym pinem interfejsowym nowo powstałego komponentu. Automatycznie numerowane.],
  [*Zegar* (`ClockComponent`)],
    [Generator sygnału zegarowego. Przy każdym „tyknięciu" (wyzwalanym automatycznie lub ręcznie) zmienia stan wyjścia na przeciwny. Umożliwia symulację układów sekwencyjnych taktowanych.],
)

== Komponenty wyjściowe

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Komponent*], [*Opis*],
  [*Wyjście* (`OutputComponent` / „Output Pin")],
    [Symboliczny pin wyjściowy. Odpowiednik `InputComponent` po stronie wyjść – definiuje interfejs komponentu własnego. Wyświetla aktualny stan sygnału.],
  [*LED* (`LEDComponent`)],
    [Dioda LED. Wizualnie zmienia kolor (aktywna/nieaktywna) w zależności od podanego sygnału. Intuicyjny wskaźnik stanu logicznego.],
)

== Przerzutniki (Flip-Flops)

Przerzutniki to podstawowe elementy *sekwencyjne* – pamiętają swój stan. Wszystkie reagują na sygnał zegarowy poprzez metodę `tickClocks()`.

#table(
  columns: (auto, auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Klasa*], [*Typ*], [*Opis*],
  [`SRFlipFlop`], [SR (Set-Reset)], [Dwa wejścia: S (Set) ustawia wyjście Q na HIGH, R (Reset) zeruje. Stan nieokreślony gdy S=R=1.],
  [`DFlipFlop`], [D (Data)], [Przy zboczu zegara przepisuje stan wejścia D na wyjście Q. Prosta „pamięć" jednego bitu.],
  [`JKFlipFlop`], [JK], [Rozszerzona wersja SR: J=K=1 powoduje przełączenie (toggle). Brak stanu nieokreślonego.],
  [`TFlipFlop`], [T (Toggle)], [Wejście T=1 przy zboczu zegara powoduje zmianę stanu wyjścia na przeciwny. T=0 – stan utrzymany.],
)

== Komponenty własne (User-Defined)

Jedną z najbardziej rozbudowanych funkcjonalności jest możliwość tworzenia *komponentów własnych (hierarchicznych)*:

1. Użytkownik projektuje dowolny układ złożony z bramek, przerzutników i innych elementów.
2. Układ musi zawierać co najmniej jeden `InputComponent` i jeden `OutputComponent` – definiują one interfejs.
3. Przez *Plik → Zapisz jako komponent* układ jest serializowany do XML, a jego definicja (`CustomComponentDefinition`) jest przechowywana przez `CustomComponentManager`.
4. Nowy komponent pojawia się w zakładce *Własne* i może być wielokrotnie używany jak każdy inny element.
5. Klasa `CustomComponent` implementuje logikę „czarnej skrzynki": przy każdym `calculate()` przepisuje stany zewnętrznych pinów do wewnętrznych `InputComponent`, uruchamia symulację wewnętrznego układu, a następnie odczytuje wartości z `OutputComponent`.

Dzięki temu możliwa jest *hierarchiczna* budowa złożonych układów (np. sumator pełny zbudowany z własnych półsumatorów).

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  4. SILNIK SYMULACJI
// ─────────────────────────────────────────────────────────────────────────────
= Silnik symulacji

== Klasa `Circuit`

`Circuit` jest centralnym obiektem aplikacji. Przechowuje:
- wektor unikalnych wskaźników do komponentów (`std::vector<std::unique_ptr<Component>>`),
- wektor unikalnych wskaźników do drutów (`Wire`),
- licznik identyfikatorów komponentów.

Główna metoda `update()` iteruje po wszystkich komponentach i wywołuje ich metodę `calculate()`, propagując sygnały przez druty. Druty są *jednokierunkowe* – wartość pinu źródłowego jest kopiowana do pinu docelowego.

== Tryby aktualizacji symulacji

Symulacja może działać w dwóch trybach konfigurowanych z poziomu UI:

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Tryb*], [*Opis*],
  [Co klatkę (interwał = 0)], [Symulacja aktualizuje się przy każdej klatce renderowania. Maksymalna prędkość – sygnały propagowane są jako szybko jak pozwala GPU/CPU.],
  [Co N sekund (interwał > 0)], [Aktualizacja następuje co zadaną liczbę sekund (0.01 – 2.0 s). Pozwala śledzić propagację sygnałów w zwolnionym tempie.],
)

Dodatkowo istnieje tryb *krokowy* – przycisk „Krok" wyzwala dokładnie jeden cykl `update()`, co ułatwia debugowanie.

== Obsługa zegarów

Zegary (`ClockComponent`) są taktowane oddzielnie od propagacji kombinacyjnej. Metoda `tickClocks()` przełącza stan wszystkich zegarów. W trybie ciągłym zegary są taktowane co 30 klatek (przy interwale = 0) lub przy każdej iteracji czasowej. Użytkownik może też ręcznie wywołać „Tyknięcie zegarów" z panelu sterowania.

== Stany maszyny skończonej układu

Klasa `Circuit` implementuje prostą maszynę stanową (`circuitState`):

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Stan*], [*Opis*],
  [`Idle`], [Spoczynek – mysz nie wykonuje żadnej akcji na układzie],
  [`DraggingComponent`], [Użytkownik przeciąga wybrany komponent po obszarze roboczym],
  [`CreatingWire`], [Użytkownik rysuje drut: kliknął pin źródłowy, oczekuje kliknięcia pinu docelowego],
)

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  5. INTERFEJS UŻYTKOWNIKA
// ─────────────────────────────────────────────────────────────────────────────
= Interfejs użytkownika

Interfejs oparty jest na bibliotece *Dear ImGui* renderowanej na oknie *SFML*. Wszystkie okna paneli są *dokowane*, *skalowalne* i mogą być ukrywane niezależnie.

== Pasek menu głównego

Pasek u góry ekranu zawiera dwa menu:

=== Menu „Plik"

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Pozycja*], [*Funkcjonalność*],
  [Zapisz (Ctrl+S)], [Otwiera natywne okno dialogowe systemu operacyjnego i zapisuje układ do pliku XML (rozszerzenie `.xml`).],
  [Otwórz (Ctrl+O)], [Otwiera natywne okno dialogowe i wczytuje układ z pliku XML. Przed wczytaniem wyświetla modal z prośbą o potwierdzenie (układ bieżący zostanie nadpisany).],
  [Zapisz jako komponent], [Serializuje bieżący układ i rejestruje go jako reużywalny komponent własny. Wymaga obecności przynajmniej jednego Input Pin i Output Pin.],
  [Wyjdź], [Zamyka aplikację.],
)

=== Menu „Widok"

Pozwala włączać i wyłączać poszczególne panele UI:
- Wybór komponentów
- Sterowanie symulacją
- Lista komponentów
- Podgląd komponentu
- Wykres pinów
- Przyciski „Pokaż wszystko" / „Ukryj wszystko"

== Panel „Wybór komponentów"

Główny panel do dodawania elementów. Podzielony na zakładki:

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Zakładka*], [*Zawartość*],
  [Bramki], [Lista rozwijana z 7 typami bramek + suwak liczby wejść (2–8). Przycisk „Dodaj bramkę".],
  [Wejścia], [Lista rozwijana: Przycisk / Wejście / Zegar. Przycisk „Dodaj wejście".],
  [Wyjścia], [Lista rozwijana: Wyjście / LED. Przycisk „Dodaj wyjście".],
  [Inne], [Lista rozwijana z 4 typami przerzutników. Przycisk „Dodaj komponent".],
  [Własne], [Lista własnych komponentów użytkownika z informacją o liczbie wejść/wyjść. Przycisk „Dodaj komponent".],
)

Panel posiada też przycisk „Wyczyść wszystko" czyszczący cały układ oraz przełącznik podglądu komponentu.

== Panel „Sterowanie symulacją"

Centralny panel kontroli biegu symulacji:

- *Checkbox „Uruchom symulację"* – start/stop ciągłej symulacji.
- *Przycisk „Krok"* – pojedynczy krok obliczeniowy.
- *Suwak „Interwał aktualizacji"* – regulacja prędkości w sekundach (0 = co klatkę, max 2 s).
- *Checkbox „Włącz zegary"* – włącza/wyłącza automatyczne taktowanie zegarów.
- *Przycisk „Tyknięcie zegarów"* – ręczny tick wszystkich zegarów + aktualizacja układu.
- *Checkbox „Rysuj wszystkie piny"* – pokazuje/ukrywa graficzne reprezentacje pinów na wszystkich komponentach.
- *Checkbox „Rysuj wszystkie etykiety"* – pokazuje/ukrywa etykiety tekstowe komponentów.
- *Ustawienia siatki*: checkbox „Pokaż siatkę", checkbox „Przyciągaj do siatki", suwak „Rozmiar siatki" (10–50 px).

== Panel „Lista komponentów"

Scrollowalna lista wszystkich komponentów obecnych w układzie. Każdy wiersz zawiera:
- etykietę komponentu (typ + numer ID),
- przycisk *„Usuń"* – natychmiastowe usunięcie komponentu i powiązanych drutów,
- przycisk *„Przejdź do"* – centruje widok kamery na wybranym komponencie.

== Panel „Podgląd komponentu"

Renderuje miniaturę aktualnie wybranego komponentu z listą rozwijanych w zakładce, pokazując jak będzie wyglądać na schemacie. Aktualizuje się na bieżąco przy zmianie wyboru w Selektorze komponentów.

== Panel „Wykres pinów" (PinPlotter)

Narzędzie do monitorowania i analizy sygnałów:

- Użytkownik prawym przyciskiem myszy wywołuje *menu kontekstowe* na dowolnym komponencie i wybiera pin (wejście lub wyjście) do monitorowania.
- Na wybranym pinie rejestrowana jest historia wartości logicznych (0/1) z kolejnych kroków symulacji.
- Każdy monitorowany pin wyświetlany jest jako zwijany nagłówek z:
  - aktualną wartością (0/1),
  - wykresem liniowym historii (ImGui `PlotLines`).
- Suwak „Próbki" reguluje długość historii (od `MIN_HISTORY` do `MAX_HISTORY` próbek).
- Przycisk „Wyczyść wszystko" usuwa wszystkie monitorowane piny z wykresu.
- Maksymalna liczba jednocześnie monitorowanych pinów jest ograniczona przez stałą `MAX_PINS`.

== Menu kontekstowe (prawy przycisk myszy)

Kliknięcie prawym przyciskiem na komponent otwiera popup z opcjami:
- *Usuń* – usuwa komponent i wszystkie połączone druty.
- *Wykres pinu...* – podmenu z listą wszystkich dostępnych pinów (wejść i wyjść) tego komponentu. Kliknięcie pinu dodaje go do wykresu PinPlotter i automatycznie otwiera okno wykresu.

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  6. OBSZAR ROBOCZY I NAWIGACJA
// ─────────────────────────────────────────────────────────────────────────────
= Obszar roboczy i nawigacja

Główna przestrzeń robocza renderowana jest przez SFML i wspiera:

== Siatka

- Opcjonalna **siatka pomocnicza** o konfigurowalnym rozmiarze oczka (10–50 px) rysowana jako linie w tle.
- Tryb **przyciągania do siatki** (`Snap to Grid`) – pozycje komponentów są zaokrąglane do najbliższego węzła siatki, ułatwiając schludne rozmieszczanie elementów.

== Przeciąganie komponentów

Komponenty można przesuwać przez *kliknięcie i przeciągnięcie* lewym przyciskiem myszy. Aplikacja oblicza offset między kursorem a centrum komponentu i płynnie aktualizuje pozycję podczas ruchu.

== Rysowanie drutów

Połączenia tworzy się przez:
1. Kliknięcie lewym przyciskiem myszy na *pin wyjściowy* źródłowego komponentu (wejście w tryb `CreatingWire`).
2. Kliknięcie lewym przyciskiem myszy na *pin wejściowy* docelowego komponentu – drut zostaje utworzony.
3. Kliknięcie prawym przyciskiem myszy anuluje rysowanie druta.

== Zoom i panoramowanie

- *Kółko myszy* przewija lub przybliża/oddala widok obszaru roboczego.

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  7. SERIALIZACJA
// ─────────────────────────────────────────────────────────────────────────────
= Serializacja

Klasa `CircuitSerializer` umożliwia trwały zapis i odczyt projektów:

#table(
  columns: (auto, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Metoda*], [*Opis*],
  [`saveToFile()`], [Zapisuje pełny stan układu (komponenty, ich pozycje, parametry i połączenia drutów) do pliku `.xml`.],
  [`loadFromFile()`], [Wczytuje układ z pliku XML, odtwarza wszystkie komponenty i druty. Sprawdza wersję pliku.],
  [`saveToXmlString()`], [Serializuje układ do łańcucha XML w pamięci – używane wewnętrznie przy zapisywaniu komponentów własnych.],
  [`loadFromXmlString()`], [Deserializuje układ z łańcucha XML – używane przy instancjonowaniu komponentów własnych.],
)

Format XML zawiera:
- wersję schematu (`currentVersion`) dla kompatybilności wstecznej,
- listę komponentów z ich typem, ID, pozycją i parametrami,
- listę połączeń (drutów) z identyfikatorami źródłowych i docelowych pinów.

Komponenty własne są persystowane do osobnego pliku XML przez `CustomComponentManager`, co oznacza, że są dostępne między sesjami programu.

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  8. FABRYKA KOMPONENTÓW
// ─────────────────────────────────────────────────────────────────────────────
= Fabryka komponentów

Klasa `ComponentFactory` implementuje wzorzec projektowy *Factory Method*. Metoda statyczna `create(typeName, id, position, numInputs)` przyjmuje nazwę tekstową typu i zwraca gotowy obiekt komponentu opakowany w `std::unique_ptr<Component>`.

Dzięki temu serializator może odtworzyć dowolny komponent po przeczytaniu jego nazwy z pliku XML, bez bezpośredniego uzależnienia od konkretnych klas. Łańcuch `if-else` wewnątrz fabryki obsługuje wszystkie wbudowane typy.

Dla komponentów własnych istnieje oddzielna metoda `createCustom(id, position, definitionName)`, która wyszukuje definicję w `CustomComponentManager` i tworzy instancję `CustomComponent`.

#pagebreak()

// ─────────────────────────────────────────────────────────────────────────────
//  9. PODSUMOWANIE
// ─────────────────────────────────────────────────────────────────────────────
= Podsumowanie

LogicGateSim dostarcza kompletne środowisko do nauki i eksperymentowania z układami cyfrowymi. Poniższa tabela zbiera wszystkie zaimplementowane funkcjonalności:

#table(
  columns: (0.4fr, 1fr),
  stroke: 0.5pt,
  fill: (col, row) => if row == 0 { luma(230) } else { white },
  [*Kategoria*], [*Funkcjonalności*],
  [Bramki], [AND, OR, NOT, NAND, NOR, XOR, XNOR; konfigurowalny 2–8 wejść],
  [Elementy wejściowe], [Przycisk (toggle), Input Pin (interfejs), Zegar (generator)],
  [Elementy wyjściowe], [Output Pin (interfejs), LED (wskaźnik)],
  [Przerzutniki], [SR, D, JK, T],
  [Komponenty własne], [Hierarchiczne, wielokrotnego użytku, persystowane między sesjami],
  [Symulacja], [Tryb ciągły (co klatkę lub co N sekund), tryb krokowy, taktowanie zegarów],
  [Obszar roboczy], [Siatka, snap-to-grid, przeciąganie komponentów, rysowanie drutów, zoom],
  [Analiza sygnałów], [PinPlotter: wykresy historii stanów wybranych pinów w czasie rzeczywistym],
  [Serializacja], [Zapis/odczyt układów XML, zapis komponentów własnych do XML],
  [Interfejs], [Pasek menu, 5 paneli dokowanych, menu kontekstowe, powiadomienia statusu],
)

Projekt stanowi solidną bazę dla dalszego rozwoju – možliwe rozszerzenia to m.in. obsługa magistral wielobitowych, symulator timingu, generowanie tablic prawdy czy eksport schematów do SVG.
