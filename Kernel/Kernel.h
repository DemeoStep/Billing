#ifndef KERNEL_H
#define KERNEL_H

class Abonent;
class Tarif;
class MySQL;
class Street;

Abonent* Abonents = NULL;
Tarif* Tarifs = NULL;
Street* Streets = NULL;
MySQL* Connection = NULL;

time_t now = time(NULL);
tm ltm;

#endif
