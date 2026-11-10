#set page(paper: "a4", numbering: "1", margin: 4em)
#show heading: set block(below: 2em)

#import "@preview/codly:2.0.0" as codly
#import "@preview/codly-languages:0.1.8": codly-languages
#show: codly.lang-set_(languages: codly-languages)
#show raw.where(block: true): codly.new

#show title: set align(center)
#show title: set block(below: 3em)
#title()[
  Sisteme de operare \
  Laborator 6
]

= Linux
#raw(read("prog-linux.cpp"), lang: "cpp", block: true)
#image("images/linux-run.png")

== Dificultati
La prima rulare programul s-a blocat deoarece incercam sa folosesc `sem_close` pentru a inchide semaforul, insa functia corecta era `sem_post`.

= Windows
//#raw(read("prog-windows.cpp"), lang: "cpp", block: true))
//#image("images/windows-run.png")