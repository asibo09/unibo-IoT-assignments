import javax.swing.SwingUtilities;

/**
 * DRU controller class.
 */
public class DruController extends Thread {
    private CommChannel channel;
    private DruView view;

    /**
     * Constructor of DruController class.
     * 
     * @param portName serial port name
     * @param view     DRU view
     */
    public DruController(String portName, DruView view) throws Exception {
        this.view = view;
        this.channel = new SerialCommChannel(portName, 9600);

        System.out.println("In attesa che Arduino si riavvii...");
        Thread.sleep(2000);

        this.view.addTakeOffListener(e -> {
            channel.sendMsg("TAKE_OFF\n");
            view.addLog("Inviato comando TAKE_OFF");
        });

        this.view.addLandingListener(e -> {
            channel.sendMsg("LANDING\n");
            view.addLog("Inviato comando LANDING");
        });
    }

    /**
     * Run method that manages the drone states and the communication with the
     * drone.
     */
    @Override
    public void run() {
        view.addLog("Connessione Seriale Stabilita!");

        while (true) {
            try {
                if (channel.isMsgAvailable()) {
                    String msg = channel.receiveMsg();

                    // ricezione messaggi "STATE:" e decodifica prendendo solo lo stato passato
                    if (msg.startsWith("STATE:")) {
                        String rawState = msg.replace("STATE:", "").trim();

                        SwingUtilities.invokeLater(() -> {
                            if (rawState.equals("DRONE INSIDE")) {
                                view.updateDroneState("rest");
                                view.updateHangarState("normal");
                                view.updateDistance("--");
                            } else if (rawState.equals("TAKE_OFF")) {
                                view.updateDroneState("taking off");
                            } else if (rawState.equals("DRONE OUT")) {
                                view.updateDroneState("operating");
                            } else if (rawState.equals("LANDING")) {
                                view.updateDroneState("landing");
                                // controlli FSM temp
                            } else if (rawState.equals("NORMAL")) {
                                view.updateHangarState("normal");
                            } else if (rawState.equals("PRE_ALARM")) {
                                view.updateHangarState("pre-alarm");
                            } else if (rawState.equals("ALARM")) {
                                view.updateHangarState("alarm");
                            }
                        });
                        // ricezione messaggi "LOG:" e aggiunta log alla console
                    } else if (msg.startsWith("LOG:")) {
                        String log = msg.replace("LOG:", "").trim();
                        SwingUtilities.invokeLater(() -> view.addLog(log));
                        // ricezione messaggi "DIST:" e aggiornamento distanza
                    } else if (msg.startsWith("DIST:")) {
                        String dist = msg.replace("DIST:", "").trim();
                        SwingUtilities.invokeLater(() -> view.updateDistance(dist));
                    }
                }
                Thread.sleep(10);
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
    }
}