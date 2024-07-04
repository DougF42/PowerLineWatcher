Power Line Monitor
* This monitors a 'main' power line, and when it sees a non-trivial load, it drives a solid state relay whioch operates an auxillary circuit.

* The USE CASE:  Is a table saw on the 'main' line, with the vaccum system on the auxilary power line. Both in this case are on a 120VAC circuit.

* This code uses an ACS712 current monitor (the 30 amp version was used) to monitor the 'main' circuit.
* An apprpriatly rated Solid State relay is used to control the auxillary power line circuit.
* ACTIONS:
* * When we detect noticable current (the table saw turns on), we turn on the relay.
* * When we detect the main current drop below a threashold (the table saw turns off), we delay aprox 2 secs (to clear the vaccum system of debree) and turn off the auxilary circuit relay.

OTHER DETAILS:
* This was initially developed and tested for an ESP32 'doitkit v1' platform. As of this writing, an Arduino (P328) based version is being developed
* 
