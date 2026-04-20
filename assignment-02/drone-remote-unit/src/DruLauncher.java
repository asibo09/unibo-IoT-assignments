/**
 * Main class of DRU system
 */
public class DruLauncher {
    /**
     * Main method.
     * 
     * @param args command line arguments
     */
    public static void main(String[] args) {
        try {
            String serialPort = "COM3";

            DruView view = new DruView();
            DruController controller = new DruController(serialPort, view);

            view.display();
            controller.start();

        } catch (Exception e) {
            System.err.println("Errore di avvio: Assicurarsi che Arduino sia collegato e la porta sia corretta!");
            e.printStackTrace();
        }
    }
}