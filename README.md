# OOP-project
Proiect pentru POO

Sistem de gestionare a bazelor de date cu indexare cu BTree si interfata grafica

## Utilizare
Dupa fiecare comanda se apasa enter
- Crearea unui tabel:
  - Se introduce numele tabelului dorit si, pe urmatoarea linie, comanda "create"
  - Userul este intrebat daca doreste ca tabelul sa aiba o cheie primara
  - Pentru fiecare coloana se introduce numele ei si, pe urmatoarea linie, tipul de date.<br/>
    Obs: Numele tipurilor de date sunt "int", "float" si "char(<numar_de_caractere>)", <br/>
    unde <numar_de_caractere> este un numar intreg
  - La ultima coloana, dupa tipul de date se introduce caracterul ";"
<img src="https://user-images.githubusercontent.com/69697694/149380186-c19981f3-6dd6-4028-a518-867a16c55ff2.png" height=50% width=50%>
    
- Adaugarea unei linii in tabel:
  - Se introduce numele tabelului dorit si, pe urmatoarea linie, comanda "insert"
  - Pentru fiecare coloana din tabel este ceruta o valoare. Promptul indica numele si<br/>
    tipul de date el coloanei
<img src="https://user-images.githubusercontent.com/69697694/149381173-1e87bac6-eb7a-402c-a699-48f6ca6fda80.png" height=50% width=50%>

- Afisarea liniilor din tabel
  - Se introduce numele tabelului dorit si, pe urmatoarea linie, comanda "select"
  - Se introduce numele coloanei dupa care vor fi filtrate liniile din tabel si apoi valoarea dorita.
<img src="https://user-images.githubusercontent.com/69697694/149381671-2a1dfe51-0cee-4a6c-a5db-d4d99c10c090.png" height=50% width=50%>

- Stergerea liniilor din tabel
  - Se introduce numele tabelului dorit si, pe urmatoarea linie, comanda "delete"
  - Se introduce numele coloanei dupa care vor fi filtrate liniile din tabel si apoi valoarea dorita.
  - Toate liniile care indeplinesc conditia de cautare vor fi sterse din tabel.
<img src="https://user-images.githubusercontent.com/69697694/149383109-b878ac4c-115f-4c3f-aab7-abe32c40b4be.png" height=50% width=50%>

- Actualizarea datelor anumitor linii:
  - Se introduce numele tabelului dorit si, pe urmatoarea linie, comanda "update"
  - Se introduce numele coloanei dupa care vor fi filtrate liniile din table si apoi valoarea dorita.
  - Se introduce numele coloanei care va fi actualizata si apoi noua valoare.
  - Toate liniile care indeplinesc conditia de cautare vor fi actualizate.
<img src="https://user-images.githubusercontent.com/69697694/149383600-39933c6e-7ff6-405a-b5b2-2b123ef73db2.png" height=50% width=50%>
