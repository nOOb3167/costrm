import locale
import logging
import subprocess

logging.basicConfig(level=logging.INFO, format="%(levelname)s:%(funcName)15s: %(message)s")

def run():
	try:
		c = subprocess.run(["PowerShell.exe", "-NoProfile", "-ExecutionPolicy", "Unrestricted", "-File", ".\\packages.ps1"], shell=True, check=True, capture_output=True, encoding=locale.getpreferredencoding())
		logging.info(f"{c.stdout}")
	except subprocess.CalledProcessError as e:
		logging.error(f"failed {e.returncode=} executing {e.cmd}")
		logging.error(f"{e.stdout}")
		logging.error(f"{e.stderr}")

if __name__ == "__main__":
	run()
