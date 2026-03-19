#set document(
  title: "LogicGateSim – Skrócony opis projektu",
  author: "PwJCpp",
)

#set page(
  paper: "a4",
  margin: (top: 2.5cm, bottom: 2.5cm, left: 2.8cm, right: 2.8cm),
)

#set text(font: "New Computer Modern", size: 11pt, lang: "pl")
#set par(justify: true, leading: 0.65em)
#set heading(numbering: "1.")

// ─── Tytuł ───────────────────────────────────────────────────────────────────
#align(center)[
  #v(0.5cm)
  #text(size: 24pt, weight: "bold")[LogicGateSim]
  #v(0.2cm)
  #text(size: 13pt, style: "italic")[Wizualny symulator układów logicznych]
  #v(0.2cm)
  #line(length: 50%, stroke: 0.5pt)
  #v(0.3cm)
  #text(size: 10pt, fill: gray)[Projekt zaliczeniowy — PwJCpp #h(1em) #datetime.today().display("[day].[month].[year]")]
  #v(0.5cm)
]

// ─── Cel projektu ────────────────────────────────────────────────────────────
= Cel projektu

*LogicGateSim* to napisana w C++ graficzna aplikacja desktopowa — symulator układów logicznych inspirowany oprogramowaniem *Multisim*. Program umożliwia interaktywne projektowanie, łączenie i symulowanie cyfrowych układów logicznych — zarówno kombinacyjnych, jak i sekwencyjnych — bez potrzeby instalowania ciężkich narzędzi inżynierskich.

Projekt skierowany jest do studentów i pasjonatów elektroniki, którzy chcą szybko przetestować działanie bramek logicznych, przerzutników czy własnych układów.

// ─── Co projekt potrafi ──────────────────────────────────────────────────────
= Główne funkcjonalności

*Biblioteka komponentów* — użytkownik może umieszczać na schemacie:
- siedem rodzajów *bramek logicznych* (AND, OR, NOT, NAND, NOR, XOR, XNOR) z regulowaną liczbą wejść (2–8),
- *elementy wejściowe*: interaktywny przycisk, pin wejściowy i generator zegarowy,
- *elementy wyjściowe*: pin wyjściowy i dioda LED reagująca na sygnał,
- cztery typy *przerzutników* (SR, D, JK, T).

*Hierarchiczne komponenty własne* — dowolny zaprojektowany układ można zapisać jako nowy, wielokrotnie używalny blok z własnym interfejsem wejść i wyjść, co pozwala budować złożone układy w sposób modularny.

*Symulacja w czasie rzeczywistym* — sygnały propagowane są automatycznie przez cały układ. Dostępne tryby: ciągły (co klatkę lub co zadany interwał czasowy) oraz krokowy (ręczne wyzwalanie jednego cyklu obliczeń).

*Analiza sygnałów* (PinPlotter) — wybrany pin dowolnego komponentu może być monitorowany w postaci wykresu historii stanów logicznych (0/1) w czasie, co ułatwia diagnozowanie zachowania układu.

*Zapis i odczyt projektów* — układ można zapisać do pliku XML i wczytać w dowolnym momencie. Komponenty własne są persystowane między sesjami.

*Wygodny obszar roboczy* — siatka z opcją przyciągania, przeciąganie komponentów myszą, rysowanie połączeń (drutów) między pinami, zoom widoku.


#align(center)[
  #v(0.8cm)
  #rect(
    width: 90%,
    stroke: 0.5pt,
    inset: 12pt,
    radius: 4pt,
  )[
    #text(style: "italic")[
      „LogicGateSim łączy prostotę obsługi z możliwościami wystarczającymi
       do nauki cyfrowych układów logicznych na poziomie akademickim."
    ]
  ]
]
