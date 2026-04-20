import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;

/**
 * DRU view class.
 */
public class DruView extends JFrame {
    private JButton btnTakeOff;
    private JButton btnLanding;
    private JLabel lblDroneState;
    private JLabel lblHangarState;
    private JLabel lblDistance;
    private JTextArea logArea;

    /**
     * Constructor of DruView class.
     */
    public DruView() {
        setTitle("DRU - Drone Remote Unit");
        setSize(550, 450);
        setLayout(new BorderLayout());
        setResizable(false);

        // buttons panel
        JPanel topPanel = new JPanel();
        btnTakeOff = new JButton("TAKE OFF");
        btnLanding = new JButton("LANDING");
        topPanel.add(btnTakeOff);
        topPanel.add(btnLanding);

        // states panel
        JPanel centerPanel = new JPanel(new GridLayout(3, 1));

        lblDroneState = new JLabel("DRONE STATE: rest", SwingConstants.CENTER);
        lblDroneState.setFont(new Font("Arial", Font.BOLD, 18));

        lblHangarState = new JLabel("HANGAR STATE: normal", SwingConstants.CENTER);
        lblHangarState.setFont(new Font("Arial", Font.BOLD, 18));
        lblHangarState.setForeground(Color.GREEN.darker());

        lblDistance = new JLabel("DISTANCE: --", SwingConstants.CENTER);
        lblDistance.setFont(new Font("Arial", Font.PLAIN, 16));
        lblDistance.setForeground(Color.BLUE);

        centerPanel.add(lblDroneState);
        centerPanel.add(lblHangarState);
        centerPanel.add(lblDistance);

        // console log
        logArea = new JTextArea(8, 40);
        logArea.setEditable(false);
        logArea.setBackground(Color.BLACK);
        logArea.setForeground(Color.GREEN);
        JScrollPane scrollPane = new JScrollPane(logArea);

        add(topPanel, BorderLayout.NORTH);
        add(centerPanel, BorderLayout.CENTER);
        add(scrollPane, BorderLayout.SOUTH);

        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    /**
     * Update drone state.
     * 
     * @param state drone state
     */
    public void updateDroneState(String state) {
        lblDroneState.setText("DRONE STATE: " + state);
    }

    /**
     * Update hangar state.
     * 
     * @param state hangar state
     */
    public void updateHangarState(String state) {
        lblHangarState.setText("HANGAR STATE: " + state);
        if (state.equals("alarm")) {
            lblHangarState.setForeground(Color.RED);
            btnTakeOff.setEnabled(false);
            btnLanding.setEnabled(false);
        } else if (state.equals("pre-alarm")) {
            lblHangarState.setForeground(new Color(255, 140, 0));
            btnTakeOff.setEnabled(false);
        btnLanding.setEnabled(false);
        } else {
            lblHangarState.setForeground(Color.GREEN.darker());
            btnTakeOff.setEnabled(true);
            btnLanding.setEnabled(true);
        }
    }

    /**
     * Update distance.
     * 
     * @param dist distance from sonar rilevator
     */
    public void updateDistance(String dist) {
        lblDistance.setText("DISTANCE: " + dist + " m");
    }

    /**
     * Add log.
     * 
     * @param log the log message
     */
    public void addLog(String log) {
        logArea.append("> " + log + "\n");
        logArea.setCaretPosition(logArea.getDocument().getLength());
    }

    /**
     * Add take off listener.
     * 
     * @param l the listener
     */
    public void addTakeOffListener(ActionListener l) {
        btnTakeOff.addActionListener(l);
    }

    /**
     * Add landing listener.
     * 
     * @param l the listener
     */
    public void addLandingListener(ActionListener l) {
        btnLanding.addActionListener(l);
    }

    /**
     * Display view.
     */
    public void display() {
        setVisible(true);
    }
}