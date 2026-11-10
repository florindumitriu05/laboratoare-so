#set page(paper: "a4", numbering: "1", margin: 4em)
#show heading: set block(below: 2em)

#show title: set align(center)
#show title: set block(below: 3em)
#title()[
  Sisteme de operare \
  Laborator 3
]

#let compare(img_linux, body_linux, img_windows, body_windows, img_size: 55%) = {
  set align(center)
  set par(justify: true)
  set heading(numbering: none)

  grid(columns: 2, gutter: 2em,
  [
    == Linux
    #image(img_linux, height: img_size) \
    #body_linux
  ],
  [
    == Windows
    #image(img_windows, height: img_size) \
    #body_windows
  ])
}

= Creare proiect nou
#image("images/Screenshot From 2026-10-20 18-46-59.png")

= Windows

+ #image("images/3-windows.png")
+ #image("images/4a-windows.png")
+ #image("images/4b-windows.png")
  Deoarece era prima data cand am folosit `git` pe PC-ul personal, am fost nevoit sa imi introduc email-ul si numele pentru a putea face un commit.

#pagebreak()
= Linux

#image("images/2si5-linux.png")
+ Am clonat repository-ul gol
+ Am executat "git pull" dupa ce am dat push la programul test de pe Windows.
+ Am compilat si executat programul test.