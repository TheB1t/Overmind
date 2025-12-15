class CfgPatches {
	class overmind_client {
		projectName = "Overmind Client";
		author = "Bit";
		version = "v0.1";
		requiredAddons[] = {};
		units[] = {};
	};
};

class CfgFunctions {
	class overmind_client {
		class basic {
			file = "overmind_client\basic";
			class version {};
		};

		class system {
			file = "overmind_client\system";
			class isLoaded {};
			class preStart {
				preStart = 1;
			};
		};
	};
};
