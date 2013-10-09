import g4p_controls.*;
import com.codeminders.hidapi.HIDDeviceInfo;
import com.codeminders.hidapi.HIDManager;
import com.codeminders.hidapi.HIDDevice;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import java.awt.Font;

GTextArea outputField;
GTextField inputField;
GButton sendButton;
GButton connectButton;
GButton pauseButton;

boolean paused;
String serialText = new String();

int VENDOR_ID = 0x16c0;
int PRODUCT_ID = 0x05df;
HIDDevice device = null;
Boolean device_initialized = false;

public void setup() {
  size(405, 500);
  G4P.setGlobalColorScheme(5);  
  outputField = new GTextArea(this, 0, 100, 405, 400, G4P.SCROLLBARS_VERTICAL_ONLY);
  // Set some default text
  outputField.setText("Connect to device first.\n");
  outputField.setDefaultText("Connect to device first.\n");
  outputField.setTextEditEnabled(true);
  outputField.setFont(new Font("Monospaced", Font.PLAIN, 12));
  inputField = new GTextField(this, 10, 50, 280, 20);
  inputField.setText("");
  inputField.setDefaultText("");
  inputField.setFont(new Font("Monospaced", Font.PLAIN, 12));
  
  GLabel outputLabel = new GLabel(this, 10, 80, 60, 20, "Output:");
  outputLabel.setTextBold();

  sendButton = new GButton(this, 300, 50, 100, 20, "Send");
  sendButton.setTextBold();
  sendButton.setEnabled(false);
  
  GLabel title = new GLabel(this, 100, 10, 200, 30, "HID Serial Monitor");
  title.setTextBold();
  
  connectButton = new GButton(this, 10, 15, 100, 20, "Connect");
  connectButton.setTextBold();
  
  pauseButton = new GButton(this, 300, 15, 100, 20, "Pause");
  pauseButton.setTextBold();
  pauseButton.setEnabled(false);

  device_initialized = false;
}

public void draw() {
  background(230,230,230);
  if( device != null && !paused ) {
    String result = deviceRead();  
    if( result != null ) {
      outputField.appendText(result);
    }
  }
}

public void handleTextEvents(GEditableTextControl textarea, GEvent event) {
  if( textarea.equals(inputField) && event == GEvent.ENTERED ) {
    send();
  }
}

public void deviceInitialize() {
  if (!device_initialized) {
    device_initialized = true;
    com.codeminders.hidapi.ClassPathLibraryLoader
          .loadNativeHIDLibrary();
  }
}

public void deviceFindFirst() {
  deviceInitialize();
  HIDDeviceInfo[] infos = deviceFindAllDescriptors();

  if (infos.length > 0) {
    try {
      device = infos[0].open();
    } catch (Exception e) {
      device = null;
    }
  }  
}

public HIDDeviceInfo[] deviceFindAllDescriptors() {
    deviceInitialize();
    
    List<HIDDeviceInfo> devlist = new ArrayList<HIDDeviceInfo>();

    try {
      HIDManager hidManager = HIDManager.getInstance();

      HIDDeviceInfo[] infos = hidManager.listDevices();

      for (HIDDeviceInfo info : infos) {
        if (info.getVendor_id() == VENDOR_ID
            && info.getProduct_id() == PRODUCT_ID) {
          devlist.add(info);
        }
      }
    } catch (Exception e) {
    }

    return devlist.toArray(new HIDDeviceInfo[devlist.size()]);  
}

public String deviceRead() {
  try {
    device.disableBlocking();
    byte[] data = new byte[8];
    int read = device.read(data);
    if (read > 0) {
      String str = new String();
      for(int i=0;i<read;i++) {
        if(data[i]!=0) str+=((char)data[i]);
      }
      return str;
    } else {
      return null;
    }
  } catch(IOException ioe) {
    //ioe.printStackTrace();
    System.out.println("deviceRead error");
  }
  return null;
}

public void deviceWrite(String value) {
  char[] charArray = value.toCharArray();
  int size = (charArray.length > 32) ? 32 : charArray.length;
  byte[] data = new byte[33];
  data[0] = (byte)0;
  int i;
  for(i=0;i<size;i++) {
    data[i+1]=(byte)charArray[i];
  }
  data[i+1]=0;
  try {
    device.sendFeatureReport(data);
    //delay(250);
  } catch(Exception e) {
    e.printStackTrace();
    System.out.println("deviceWrite error");
  }
}

public void handleButtonEvents(GButton button, GEvent event) { 
  if( event == GEvent.CLICKED ) {
    if ( button.equals(sendButton) ) {
      send();
    } else if ( button.equals(connectButton) ) {
      if (device!=null) {
        pauseButton.setEnabled(false);
        sendButton.setEnabled(false);
        device = null;
        connectButton.setText("Connect");
        connectButton.setTextBold();
        outputField.appendText("Disconnected.\n");
      } else {
        deviceFindFirst();
        if(device == null) {
          outputField.appendText("Could not find device!!\n");
        } else{
          connectButton.setText("Disconnect");
          connectButton.setTextBold();
          pauseButton.setEnabled(true);
          sendButton.setEnabled(true);
          outputField.appendText("Connected successfully.\n");
        }
      }
    } else if (  button.equals(pauseButton) ) {
      pauseButton.setText( (paused) ? "Pause" : "Resume" );
      pauseButton.setTextBold();
      paused = !paused;
    }   
  }
}

private void send() {
  if( device != null ) {
    String entered = inputField.getText();
    if( entered != null && !entered.equals("") ) {
      deviceWrite( inputField.getText() );
    }
    inputField.setText("");
  }
}

