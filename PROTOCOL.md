# chatd protokoll

A kliens és szerver egy saját szöveges protokollon keresztül kommunkál. A
parancsok általános formátuma a következő:

    COMMAND [arguments]

A válasz formátuma a következő:

    CODE message

Sikeres végrehajtás esetén a visszatérésis kód:

    100 message

A pontos hibakódok megtalálhatóak a `command.h` header fájlban.

## Parancsok

### Belépés

    REG username [password]

Regisztráció és authentikáció egyben. Nem létező felhasználó esetén létrehoz
egyet, ha a jelszó is van, akkor perzisztens felhasználót hoz létre, aki később
is beléphet. Ha nincs jelszo, akkor ideiglenes felhasználó jön létre, ami a
sessionje végén megszűnik.

### Kilépés

    EXIT

Kilép a felhasználó. Minden csatornáról kiléptetjük.

### Jelszó csere/létrehozás

    PWD new [old]

Jelszó csere. Amennyiben a felhasználó csak ideiglenes és nincs még jelszava a
szerveren, akkor regisztrálja a szerver a felhasználót állandó felhasználóként
regisztrálja megadott jelszóval.  Ilyenkor az _old_ paraméter megadása nem
szükges.

### Csatlakozás szobához

    JOIN token room

Amennyiben a szoba még nem létezik, akkor újat hozunk létre. Ha már létezik,
akkor csak belépünk.
