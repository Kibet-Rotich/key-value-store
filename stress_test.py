import socket
import time
import random
from concurrent.futures import ThreadPoolExecutor, as_completed

HOST = "127.0.0.1"
PORT = 8080

# The Load Parameters
NUM_WORKERS = 50       # How many clients attacking at the exact same time
TOTAL_REQUESTS = 2000  # Total number of commands to send

def single_client_task(task_id):
    """Simulates a single client connecting, sending a command, and disconnecting."""
    try:
        # Create a new connection for this specific task
        with socket.create_connection((HOST, PORT), timeout=3) as client:
            # Consume the banner message
            client.recv(1024)
            
            # Randomly decide whether to SET or GET to simulate real traffic
            action = random.choice(["SET", "GET"])
            key = f"key_{task_id % 100}" # Reuse 100 keys to force collisions and overwrites
            
            if action == "SET":
                message = f"SET {key} value_from_task_{task_id}\n"
            else:
                message = f"GET {key}\n"
            
            client.sendall(message.encode("utf-8"))
            response = client.recv(1024).decode("utf-8", errors="replace").strip()
            
            return True, response
    except Exception as e:
        return False, str(e)

def main():
    print(f"🔥 Starting Stress Test on {HOST}:{PORT} 🔥")
    print(f"Workers (Concurrent Threads): {NUM_WORKERS}")
    print(f"Total Requests: {TOTAL_REQUESTS}\n")
    
    success_count = 0
    fail_count = 0
    start_time = time.time()

    # Launch the multi-threaded attack
    with ThreadPoolExecutor(max_workers=NUM_WORKERS) as executor:
        # Submit all tasks to the thread pool
        futures = [executor.submit(single_client_task, i) for i in range(TOTAL_REQUESTS)]
        
        # Process results as they finish
        for future in as_completed(futures):
            success, result = future.result()
            if success:
                success_count += 1
            else:
                fail_count += 1
                # ADD THIS LINE TO SEE THE ERROR:
                print(f"Request failed: {result}")
                
            # Print a progress indicator every 500 requests
            if (success_count + fail_count) % 500 == 0:
                print(f"Progress: {success_count + fail_count} / {TOTAL_REQUESTS} completed...")

    end_time = time.time()
    duration = end_time - start_time
    req_per_sec = TOTAL_REQUESTS / duration if duration > 0 else 0

    print("\n" + "="*40)
    print("📊 STRESS TEST RESULTS 📊")
    print("="*40)
    print(f"Time Taken:      {duration:.2f} seconds")
    print(f"Requests/Sec:    {req_per_sec:.2f}")
    print(f"Successful:      {success_count}")
    print(f"Failed/Errors:   {fail_count}")
    print("="*40)

if __name__ == "__main__":
    main()