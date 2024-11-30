
extern "C" {
	/**
		Installs the plugin.

		@return - True if the installation was successful.
	 */
	__declspec(dllexport) bool __cdecl install();

	/**
		Removes the plugin.
	 */
	__declspec(dllexport) void __cdecl uninstall();
}
