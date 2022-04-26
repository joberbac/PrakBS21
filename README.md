# PrakBS21

## Deadlines
| Meilenstein	| Datum	| Erledigt sollte sein... |√|
|---|---|---|---|
|I | 29.04.2022	| Team ist handlungsfähig, d.h. Entwicklungsumgebung läuft und ein erstes Programm mit Socket wurde entwickelt.|√|
|II	| 20.05.2022 | Zu diesem Zeitpunkt sollten Sie das Praktikum bis einschließlich der Teilaufgabe "Multiclientfähigkeit" bearbeitet haben.|√|
|III	| 10.06.2022	| Zu diesem Zeitpunkt sollten Sie das Praktikum bis einschließlich der Teilaufgabe "Pub/Sub" bearbeitet haben.||
|IV	 | 24.06.2022	| Zu diesem Zeitpunkt sollten Sie den Basis-Teil und etwaige von Ihnen bearbeitete Erweiterungen abgeschlossen und in Ihr Repository geladen haben.||
|V	| 08.07.2022	| Deadline für Teilnahmenachweis||

## Tipps zu git Terminal Befehlen
Zum Branchwechsel in das Terminal in CLion ein checkout machen.
Vorher ein Pull-Request machen, damit man alle Änderungen der anderen hat.
```
git pull

git checkout BranchName
```

## Bsp. Kommunikation auf Client-Seite
key und value akzeptieren alphanumerische Zeichen, keine Sonderzeichen.
Der key darf keine Leerzeichen enthalten.
INPUT-Format: command key value
OUTPUT-Format: command:key:value
 ```
GET key1
> GET:key1:key_nonexistent
PUT key1 value1
> PUT:key1:value1
PUT key2 value2
> PUT:key2:value2
PUT key1 value3
> PUT:key1:value3
DEL key2
> DEL:key2:key_deleted
DEL key2
> DEL:key2:key_nonexistent
GET key1
> GET:key1:value3
QUIT
 ```


## Ressourcen
- zur schnellen wdh. der Grundlagen von C: [# C Programming | In One Video](https://www.youtube.com/watch?v=3lQEunpmtRA) by Mike Dane

### Zu Meilenstein 1
- [C von A bis Z - Socket Programmierung](https://openbook.rheinwerk-verlag.de/c_von_a_bis_z/025_c_netzwerkprogrammierung_004.htm#mj50a66f07dc78c522bbc8a31b18c366a8)
- [git Repo distcmpting von amruthaarajan](https://github.com/amruthaarajan/distcmpting) - hier auch mal ein Beispiel für einen Key Value Store, der eigentlich auch schon alles kann, was wir im ersten Meilenstein auch haben sollten...

### Ausführung des Codes: (Windows)
```
in CLion: Build -> Ausführen


in Ubuntu: ssh root@localhot -> pw: root -> telnet localhost 5678 

```
### Ausführen des Codes: (Mac)
1. main.c in CLion ausführen
2. Terminal Fenster öffnen und Verbinden mit
```
telnet localhost PortNummer
```
Portnummer steht in main.c ganz oben (5678)
