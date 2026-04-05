import socket
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

HOST = "127.0.0.1"
PORT = 8080

# The Stress Parameters
NUM_WORKERS = 100
TOTAL_REQUESTS = 10000

def send_command(message: str) -> tuple[bool, str]:
    """Helper to open a connection, send one command, and return the response."""
    try:
        with socket.create_connection((HOST, PORT), timeout=5) as client:
            client.recv(1024) # Consume banner
            client.sendall(message.encode("utf-8"))
            response = client.recv(1024).decode("utf-8", errors="replace").strip()
            return True, response
    except Exception as e:
        return False, str(e)

def write_task(task_id):
    """Phase 1: Insert a unique key."""
    key = f"user_{task_id}"
    value = f"data_payload_{task_id}"
    message = f"SET {key} {value}\n"
    success, response = send_command(message)
    return task_id, success, response

def read_task(task_id):
    """Phase 2: Retrieve the exact key and verify it."""
    key = f"user_{task_id}"
    expected_value = f"data_payload_{task_id}"
    message = f"GET {key}\n"
    success, response = send_command(message)
    
    # Evaluate integrity
    if not success:
        return "NETWORK_ERROR"
    elif response == "(nil)":
        return "MISSING_DATA"
    elif response != expected_value:
        return "CORRUPTED_DATA"
    else:
        return "PERFECT"

def main():
    print(f"🔥 Starting Integrity Stress Test 🔥")
    print(f"Workers: {NUM_WORKERS} | Total Unique Keys: {TOTAL_REQUESTS}\n")
    
    # ==========================================
    # PHASE 1: MASSIVE CONCURRENT WRITES
    # ==========================================
    print(">>> PHASE 1: Writing 10,000 unique keys...")
    start_time = time.time()
    
    with ThreadPoolExecutor(max_workers=NUM_WORKERS) as executor:
        futures = [executor.submit(write_task, i) for i in range(TOTAL_REQUESTS)]
        
        for i, future in enumerate(as_completed(futures)):
            if (i + 1) % 2500 == 0:
                print(f"    Writes completed: {i + 1} / {TOTAL_REQUESTS}")

    write_time = time.time() - start_time
    print(f"✅ Write Phase Complete ({write_time:.2f} seconds)\n")

    # ==========================================
    # PHASE 2: INTEGRITY VERIFICATION
    # ==========================================
    print(">>> PHASE 2: Reading and Verifying all 10,000 keys...")
    results = {"PERFECT": 0, "MISSING_DATA": 0, "CORRUPTED_DATA": 0, "NETWORK_ERROR": 0}
    
    start_time = time.time()
    with ThreadPoolExecutor(max_workers=NUM_WORKERS) as executor:
        futures = [executor.submit(read_task, i) for i in range(TOTAL_REQUESTS)]
        
        for i, future in enumerate(as_completed(futures)):
            status = future.result()
            results[status] += 1
            if (i + 1) % 2500 == 0:
                print(f"    Reads completed: {i + 1} / {TOTAL_REQUESTS}")

    read_time = time.time() - start_time
    
    # ==========================================
    # THE RECKONING
    # ==========================================
    print("\n" + "="*45)
    print("🛡️  DATABASE INTEGRITY REPORT 🛡️")
    print("="*45)
    print(f"Total Keys Expected:   {TOTAL_REQUESTS}")
    print(f"Flawless Retrievals:   {results['PERFECT']}")
    print("-" * 45)
    print(f"❌ Missing Keys:       {results['MISSING_DATA']}  <-- Overwritten by race conditions")
    print(f"⚠️  Corrupted Values:   {results['CORRUPTED_DATA']}  <-- Memory mangled by threads")
    print(f"🔌 Network Errors:     {results['NETWORK_ERROR']}  <-- Did the C server crash?")
    print("="*45)

if __name__ == "__main__":
    main()