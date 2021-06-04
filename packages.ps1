try {
	py -m venv --clear data\venv
	if (!$?) { throw "error creating venv" }
	try {
		.\\data\\venv\\Scripts\\Activate.ps1
		if (!$?) { throw "error activating venv" }
		python -m pip install --help
		python -m pip install --target=.\\data\\pkgs --upgrade-strategy=eager --upgrade numpy
		if (!$?) { throw "error installing pkgs" }
	} finally {
		deactivate
	}
} catch {
	write-host "errored"
	write-host $_
	throw
}