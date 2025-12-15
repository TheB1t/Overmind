class CfgPatches {
	class overmind_server {
		projectName = "Overmind Server";
		author = "Bit";
		version = "v0.1";
		requiredAddons[] = {};
		units[] = {};
	};
};

class CfgFunctions {
	class overmind_server {
		class basic {
			file = "overmind_server\basic";
			class version {};
		};

		class system {
			file = "overmind_server\system";
			class isLoaded {};
			class preStart {
				preStart = 1;
			};
		};
	};
};
