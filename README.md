# CAR BLACK BOX

A **Car Black Box** is a vehicle monitoring device that records data such as speed, location, and driving behavior. It's used for accident analysis, insurance claims, and improving driver safety.

### Features Overview:

1. **Event Logging:**
   - The system records various vehicle events such as ignition on/off, gear changes, collisions, and speed.
   - Events are stored with timestamp data in EEPROM, ensuring non-volatile storage even after power loss.
   - The logged events can be viewed, downloaded, or cleared through the user interface.

2. **Real-Time Clock (RTC) Integration:**
   - The code integrates with an RTC module to maintain and display accurate time.
   - The time is retrieved using `get_time()` and is continuously updated on the display.

3. **User Interface (UI) with Password Protection:**
   - The system has a password-protected menu that allows users to access logs, change settings, and secure the system.
   - The password is stored in EEPROM and verified during login attempts.

4. **Event Detection and Dashboard Display:**
   - The main dashboard displays the current time, event type, and vehicle speed.
   - The system detects specific events through switch inputs and analog-to-digital conversion for speed monitoring.

5. **Menu Navigation:**
   - Users can navigate through a menu system with options like viewing logs, downloading logs, clearing logs, setting the time, and changing the password.
   - Short and long button presses are used to navigate and select menu options.

6. **EEPROM Interaction:**
   - The system reads from and writes to EEPROM to store events and user settings, ensuring data persistence.

7. **Modular Code Structure:**
   - Functions are modular, handling specific tasks like `get_time()`, `write_log_eeprom()`, `read_log_eeprom()`, and `init_config()`, making the code easy to maintain and extend.

### User Interaction Flow:

1. **Startup:**
   - The system initializes peripherals, reads the stored password from EEPROM, and displays the current time using RTC.

2. **Event Logging:**
   - Based on user input or vehicle events such as Collision, Up or Down gear shift the system logs events with a timestamp and current speed.

3. **Password Verification:**
   - When accessing the menu, the user must enter a correct password. After three failed attempts, access is temporarily blocked for 120 seconds.

4. **Menu Operations:**
   - Users can View, Download, or Clear logs, Set the Time, or Change the current Password through menu navigation.

5. **Data Management:**
   - Logged data can be viewed on the CLCD display or downloaded via UART communication, ensuring ease access to stored information.

This Car Black Box system effectively combines real-time monitoring, event logging, and secure user interaction to serve as a comprehensive vehicle monitoring tool.
