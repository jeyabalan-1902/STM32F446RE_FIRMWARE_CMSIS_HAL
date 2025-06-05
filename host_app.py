import serial
import time
import struct
import sys

# Bootloader commands
CMD_MEM_WRITE = 0x57
CMD_GO_TO_ADDR = 0x55
ACK = 0xA5
NACK = 0x7F

APP_ADDRESS = 0x08008000  # Should match STM32 app start address

def send_command(ser, cmd):
    ser.write(bytes([cmd]))
    ack = ser.read(1)
    if ack == bytes([ACK]):
        print(f"[OK] Command 0x{cmd:02X} acknowledged.")
        return True
    else:
        print(f"[ERR] Command 0x{cmd:02X} not acknowledged.")
        return False

def write_bin(ser, bin_path, start_addr):
    with open(bin_path, "rb") as f:
        data = f.read()

    chunk_size = 64  # Safe chunk size

    for i in range(0, len(data), chunk_size):
        chunk = data[i:i+chunk_size]
        addr = start_addr + i

        # Send MEM_WRITE command
        ser.write(bytes([CMD_MEM_WRITE]))
        time.sleep(0.01)

        # Send address
        ser.write(struct.pack("<I", addr))

        # Send data length
        ser.write(bytes([len(chunk)]))

        # Send data
        ser.write(chunk)

        # Wait for ACK/NACK
        ack = ser.read(1)
        if ack != bytes([ACK]):
            print(f"[ERR] Flash failed at address 0x{addr:08X}")
            return False
        print(f"[OK] Wrote {len(chunk)} bytes to 0x{addr:08X}")
    return True

def jump_to_app(ser, address):
    ser.write(bytes([CMD_GO_TO_ADDR]))
    time.sleep(0.01)
    ser.write(struct.pack("<I", address))
    ack = ser.read(1)
    if ack == bytes([ACK]):
        print("[OK] Jump command sent successfully.")
    else:
        print("[ERR] Jump failed.")

def main():
    if len(sys.argv) != 3:
        print("Usage: python stm32_uart_loader.py <COM_PORT> <bin_file>")
        return

    port = sys.argv[1]
    bin_file = sys.argv[2]

    ser = serial.Serial(port, baudrate=115200, timeout=2)

    print("[*] Connected to", port)

    if write_bin(ser, bin_file, APP_ADDRESS):
        print("[*] Flashing completed.")
        jump_to_app(ser, APP_ADDRESS)

    ser.close()

if __name__ == "__main__":
    main()
