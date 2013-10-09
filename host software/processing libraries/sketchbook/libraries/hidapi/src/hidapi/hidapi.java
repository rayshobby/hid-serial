/**
 * SquareWear
 *
 * @author      Cheni Chadowitz
 * @modified    08/19/2013
 * @version     0.1.0 (1)
 */

package SquareWear;

import com.codeminders.hidapi.HIDDeviceInfo;
import com.codeminders.hidapi.HIDManager;
import com.codeminders.hidapi.HIDDevice;

import java.io.IOException;
import java.util.List;
import java.util.ArrayList;

/**
 * This is a template class and can be used to start a new processing library or
 * tool. Make sure you rename this class as well as the name of the example
 * package 'template' to your own library or tool naming convention.
 *
 * @example Hello
 *
 *          (the tag @example followed by the name of an example included in
 *          folder 'examples' will automatically include the example in the
 *          javadoc.)
 *
 */

public class SquareWear {

	public final static String VERSION = "0.1.0";

	private static Boolean Initialized = false;

	/**
	 * SquareWear vendor ID
	 */
	public final static int VENDOR_ID = 0x16c0;

	/**
	 * SquareWear product ID
	 */
	public final static int PRODUCT_ID = 0x05df;

	/**
	 * HID device object to communicate directly with SquareWear
	 */
	private HIDDevice device = null;

	/**
	 * return the version of the library.
	 *
	 * @return String
	 */
	public static String version() {
		return VERSION;
	}

	/**
	 * Assign HIDDevice
	 *
	 * @param device HID device object to communicate directly with SquareWear
	 */
	private void setDevice(HIDDevice device) {
		this.device = device;
	}

	/**
	 * Load hidapi library based on the OS. This function is automatically called whenever
	 * a search for SquareWear is performed for the first time.
	 */
	public static void Initialize() {
		if (!Initialized) {
			Initialized = true;

			com.codeminders.hidapi.ClassPathLibraryLoader
					.loadNativeHIDLibrary();
		}
	}

	/**
	 * Find first SquareWear connected to the computer
	 *
	 * @return SquareWear object or null if no SquareWears are connected
	 */
	public static SquareWear findFirst() {
		Initialize();

		HIDDeviceInfo[] infos = findAllDescriptors();

		if (infos.length > 0) {
			SquareWear result = new SquareWear();
			try {
				result.setDevice(infos[0].open());
				return result;
			} catch (Exception e) {
			}
		}
		return null;
	}

	/** Find SquareWear by serial number
	 *
	 * @param serial	The serial number to search
	 *
	 * @return			SquareWear object or null if device was not found
	 */
	public static SquareWear findBySerial(String serial) {
		Initialize();

		HIDDeviceInfo[] infos = findAllDescriptors();

		for (HIDDeviceInfo info : infos) {
			if (info.getSerial_number().equals(serial)) {
				SquareWear result = new SquareWear();
				try {
					result.setDevice(infos[0].open());
					return result;
				} catch (Exception e) {
				}
			}
		}

		return null;
	}

	/**
	 * Find all SquareWear HIDDeviceInfo descriptions connected to the computer
	 *
	 * @return an array of HIDDeviceInfo objects with VID and PID matching SquareWear
	 */
	private static HIDDeviceInfo[] findAllDescriptors() {
		Initialize();

		List<HIDDeviceInfo> SquareWearList = new ArrayList<HIDDeviceInfo>();

		try {
			HIDManager hidManager = HIDManager.getInstance();

			HIDDeviceInfo[] infos = hidManager.listDevices();

			for (HIDDeviceInfo info : infos) {
				if (info.getVendor_id() == VENDOR_ID
						&& info.getProduct_id() == PRODUCT_ID) {
					SquareWearList.add(info);
				}
			}
		} catch (Exception e) {
		}

		return SquareWearList.toArray(new HIDDeviceInfo[SquareWearList.size()]);
	}

	/**
	 * Find all SquareWears connected to the computer
	 *
	 * @return an array of SquareWear objects
	 */
	public static SquareWear[] findAll() {
		List<SquareWear> SquareWearList = new ArrayList<SquareWear>();

		HIDDeviceInfo[] infos = findAllDescriptors();

		for (HIDDeviceInfo info : infos) {
			SquareWear SquareWear = new SquareWear();
			try {
				SquareWear.setDevice(info.open());
				SquareWearList.add(SquareWear);
			} catch (Exception e) {
			}
		}

		return SquareWearList.toArray(new SquareWear[SquareWearList.size()]);
	}

	public static void main(String[] args) {
		SquareWear s = SquareWear.findFirst();
		s.write("abcdefgh");
		while(true) {
			String res = s.read();
			if(res != null) System.out.print(res);
		}
	}
	/**
	 * Get value of InfoBlocks
	 *
	 * @param id	InfoBlock id, should be 1 or 2 as only supported info blocks
	 */
	public String getInfoBlock(int id) {
		byte[] data = new byte[33];
		data[0] = (byte) (id + 1);

		String result = "";
		try {
			int read = device.getFeatureReport(data);
			System.out.println("Read: "+read);
			System.out.println("0: "+data[0]);
			if (read > 0) {
				for (int i = 1; i < data.length; i++) {
					//System.out.println(i+": "+(char)data[i]);
					if (i == 0) {
						break;
					}

					result += (char) data[i];
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	/**
	 * Read 8 bytes of data from the device
	 *
	 * @return String of up to 8 chars, or null
	 */
	public String read() {
		try {
			device.disableBlocking();
			byte[] data = new byte[8];
			int read = device.read(data);
			if(read > 0) {
				String str = new String();
				for(int i=0; i<read; i++) {
					if(data[i] != 0) {
						str += data[i];dasdf
					}
				}
				return str;
			} else {
				return null;
			}
		} catch(IOException ioe) {
			ioe.printStackTrace();
		}
		return null;
	}
	/**
	 * Writes up to 32 bytes of data to the device
	 * @param value
	 */
	public void write(String value) {
		char[] charArray = value.toCharArray();
		byte[] data = new byte[32];
		data[0] = (byte) 1;
		for( int i=1; i < charArray.length; i++ ) {
			if(i >= 32) { // cannot send more than 32 bytes as of now
				break;
			}
			data[i] = (byte) charArray[i-1];
		}
		try {
			System.out.println("SENT: "+device.sendFeatureReport(data));
		} catch (Exception e) {
			e.printStackTrace();

		}
	}
	/**
	 * Set value for InfoBlocks
	 *
	 * @param id	InfoBlock id, should be 1 or 2 as only supported info blocks
	 * @param value	The value to be written to the info block
	 */
	public void setInfoBlock(int id, String value) {
		char[] charArray = value.toCharArray();
		byte[] data = new byte[33];
		data[0] = (byte) (id + 1);

		for (int i = 0; i < charArray.length; i++) {
			if (i > 32) {
				break;
			}

			data[i + 1] = (byte) charArray[i];
		}

		try {
			device.sendFeatureReport(data);
		} catch (Exception e) {
			e.printStackTrace();

		}
	}

	/**
	 * Get the manufacturer of the device
	 *
	 * @return Returns the manufacturer name of the device
	 */
	public String getManufacturer() {
		try {
			return device.getManufacturerString();
		} catch (Exception e) {
			return "";
		}
	}

	/**
	 * Get the product description of the device
	 *
	 * @return Returns the product name of the device.
	 */
	public String getProduct() {
		try {
			return device.getProductString();
		} catch (Exception e) {
			return "";
		}
	}


	/**
	 * Get the serial number of the device
	 *
	 * @return Returns the serial number of device.
	 */
	public String getSerial() {
		try {
			return device.getSerialNumberString();
		} catch (Exception e) {
			return "";
		}
	}

}
