# 🚀 Embedded Systems & IoT - UNIBO
*Raccolta assignment per il corso di Embedded System and IoT (A.A. 2025/2026), Ingegneria e Scienze Informatiche, Università di Bologna - Cesena.*
Il repository contiene tre assignment che esplorano lo sviluppo di sistemi embedded: dalla manipolazione hardware di basso livello fino ad architetture IoT distribuite.

## 📂 Struttura dei Progetti

### 🎮 [Assignment #01: Turn on the Sequence! (TOS)](./assignment-01)
Un gioco interattivo basato su Arduino Uno. Il progetto si concentra sull'interazione hardware diretta e sull'ottimizzazione energetica.
* **Architettura:** Super-Loop.
* **Concetti Chiave:** Gestione del *Deep Sleep*, utilizzo di *Interrupts*, *Debouncing*.
* **Hardware:** Arduino Uno, LED, Pulsanti, Potenziometro, Display LCD I2C.

---

### 🚁 [Assignment #02: Smart Drone Hangar](./assignment-02)
Il sistema di controllo automatizzato per un Hangar di Droni. Questo assignment passa a un sistema concorrente e modulare.
* **Architettura:** Task-Based con *Cooperative Scheduler*.
* **Concetti Chiave:** Modellazione a oggetti (OOP) in C++, Macchine a Stati Finiti (FSM) Sincrone, comunicazione Seriale.
* **Componenti:**
  * `drone-hangar`: Sottosistema fisico su Arduino che gestisce Servo, Led, LCD e sensori Sonar, Pir, Temp. L'architettura è basata su Task concorrenti gestiti da uno scheduler e implementa logiche tramite Macchine a Stati Finiti (FSM) sincrone.
  * `drone-remote-unit`: Interfaccia grafica per PC in Java che comunica con l’Hangar tramite Seriale.
    
**[📄 Report Tecnico #02](./assignment-02/doc/report.pdf)**

---

### 💧 [Assignment #03: Smart Tank Monitoring System](./assignment-03)
Un sistema IoT distribuito per il monitoraggio e la gestione remota (WCS, TMS, CUS, DBS). Il progetto fa da ponte tra il mondo dell'embedded puro e i protocolli web.
* **Architettura di Sistema:**
  * **WCS (Water Channel Subsystem - Arduino):** Basato su Arduino Uno, sistema embedded che controlla il canale idrico che collega il serbatoio alla rete dei canali idrici. Interagisce tramite Seriale con il CUS. Gestisce l'attuatore fisico con un'architettura *Task-Based* per gestire un Servomotore che funge da valvola, mostrare i dati su un LCD I2C e permette il controllo manuale tramite potenziometro e pulsante. 
  * **TMS (Telemetry Monitoring Subsystem - ESP32):** Basato su ESP32-S3, monitora il livello dell'acqua nel serbatoio tramite sonar e comunica lo stato via Wi-Fi tramite protocollo MQTT.
  * **CUS (Control Unit Subsystem - Java):** Applicazione in Java, si tratta del sottosistema principale che governa e coordina l’intero sistema. Riceve i dati MQTT dal TMS per il monitoraggio, elabora le Macchine a Stati e comunica via Seriale con il WCS. Funge da server HTTP per il DBS.
  * **DBS (Dashboard Subsystem - Web):** Una Web App per operatori remoti. Interroga il CUS tramite HTTP e mostra un'interfaccia utente in tempo reale per visualizzare dati e interagire col sistema tramite l’invio di comandi manuali.
* **Tecnologie:** Arduino Uno, ESP32, protocollo MQTT, Java, e una Dashboard Web (HTML/CSS) per il monitoraggio dei dati in tempo reale.
  
**[📄 Report Tecnico #03](./assignment-03/doc/report.pdf)**

---

## 🛠️ Tecnologie e Strumenti
* **Linguaggi:** C++, Java, HTML/CSS.
* **Piattaforme:** Arduino, ESP32.
* **Protocolli & Librerie:** UART (Seriale), I2C, MQTT, Vertx.

## 👤 Autore
**Pietro Siboni**
