#set page(paper: "a4", numbering: "1", margin: 4em)
#show heading: set block(below: 2em)

#show title: set align(center)
#show title: set block(below: 3em)
#title()[
  Sisteme de operare \
  Laborator 4
]

= Linux
#image("images/Screenshot From 2026-10-27 18-17-58.png")

#block(breakable: false,
[
  #set block(below: 0pt)
  = Windows
  #image("images/code-windows-1.png")
  #image("images/code-windows-2.png")
])

O dificultate pe care am intampinat-o a fost cand am incercat sa folosesc `msys2` pentru a compila programul pe windows, iar rezolvarea a fost sa instalez `Visual Studio` si sa folosesc compilatorul `MSVC`.
#image("images/run-windows.png")