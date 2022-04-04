//
// Created by Jacqueline Oberbach on 04.04.22.
//

#include "sub.h"

/*
 * TODO put()
 * Die put() Funktion soll eine Wert (value) mit dem Schlüsselwert (key) hinterlegen.
 * Wenn der Schlüssel bereits vorhanden ist, soll der Wert überschrieben werden.
 * Der Rückgabewert der Funktion könnte Auskunft dazu geben.
 */
int put(char* key, char* value);

/*
 * TODO get()
 * Die get() Funktion soll einen Schlüsselwert (key) in der Datenhaltung suchen
 * und den hinterlegten Wert (value) zurückgeben.
 * Ist der Wert nicht vorhanden, wird durch einen Rückgabewert <0 darauf aufmerksam gemacht.
 * */
int get(char* key, char* res);

/*
 * TODO del()
 * Die del() Funktion soll einen Schlüsselwert suchen und zusammen mit dem Wert aus der Datenhaltung entfernen.
 */
int del(char* key);
