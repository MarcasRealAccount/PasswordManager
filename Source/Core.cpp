#include "Core.h"

namespace Core {
	std::ostream& printPMConfigFlags(std::ostream& stream, const PMConfigFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		PMConfigFlags value = flags;
		if (value == PMConfigFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & PMConfigFlag::Debug) == PMConfigFlag::Debug) {
				str << "Debug";
				value -= PMConfigFlag::Debug;
				hasAppended = true;
			}
			if ((value & PMConfigFlag::Dist) == PMConfigFlag::Dist) {
				if (hasAppended)
					str << " | ";
				str << "Dist";
				value -= PMConfigFlag::Dist;
				hasAppended = true;
			}
			if (value != 0) {
				if (hasAppended)
					str << " | ";
				str << value;
			}
		}

		return stream << str.str();
	}

	std::ostream& printPMSystemFlags(std::ostream& stream, const PMSystemFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		PMSystemFlags value = flags;
		if (value == PMSystemFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & PMSystemFlag::Windows) == PMSystemFlag::Windows) {
				str << "Windows";
				value -= PMSystemFlag::Windows;
				hasAppended = true;
			}
			if ((value & PMSystemFlag::Macosx) == PMSystemFlag::Macosx) {
				if (hasAppended)
					str << " | ";
				str << "Macosx";
				value -= PMSystemFlag::Macosx;
				hasAppended = true;
			}
			if ((value & PMSystemFlag::Linux) == PMSystemFlag::Linux) {
				if (hasAppended)
					str << " | ";
				str << "Linux";
				value -= PMSystemFlag::Linux;
				hasAppended = true;
			}
			if ((value & PMSystemFlag::Unix) == PMSystemFlag::Unix) {
				if (hasAppended)
					str << " | ";
				str << "Unix";
				value -= PMSystemFlag::Unix;
				hasAppended = true;
			}
			if (value != 0) {
				if (hasAppended)
					str << " | ";
				str << value;
			}
		}

		return stream << str.str();
	}

	std::ostream& printPMToolsetFlags(std::ostream& stream, const PMToolsetFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		PMToolsetFlags value = flags;
		if (value == PMToolsetFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & PMToolsetFlag::MSVC) == PMToolsetFlag::MSVC) {
				str << "MSVC";
				value -= PMToolsetFlag::MSVC;
				hasAppended = true;
			}
			if ((value & PMToolsetFlag::Clang) == PMToolsetFlag::Clang) {
				if (hasAppended)
					str << " | ";
				str << "Clang";
				value -= PMToolsetFlag::Clang;
				hasAppended = true;
			}
			if ((value & PMToolsetFlag::GCC) == PMToolsetFlag::GCC) {
				if (hasAppended)
					str << " | ";
				str << "GCC";
				value -= PMToolsetFlag::GCC;
				hasAppended = true;
			}
			if (value != 0) {
				if (hasAppended)
					str << " | ";
				str << value;
			}
		}

		return stream << str.str();
	}

	std::ostream& printPMPlatformFlags(std::ostream& stream, const PMPlatformFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		PMPlatformFlags value = flags;
		if (value == PMPlatformFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & PMPlatformFlag::X86) == PMPlatformFlag::X86) {
				str << "X86";
				value -= PMPlatformFlag::X86;
				hasAppended = true;
			}
			if ((value & PMPlatformFlag::AMD64) == PMPlatformFlag::AMD64) {
				if (hasAppended)
					str << " | ";
				str << "AMD64";
				value -= PMPlatformFlag::AMD64;
				hasAppended = true;
			}
			if (value != 0) {
				if (hasAppended)
					str << " | ";
				str << value;
			}
		}

		return stream << str.str();
	}
} // namespace Core
