#include "Core.h"

namespace Core {

	std::ostream& operator<<(std::ostream& stream, const EPMConfigFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		std::uint16_t value = flags.m_Value;
		if (value == EPMConfigFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & EPMConfigFlag::Debug) == EPMConfigFlag::Debug) {
				str << "Debug";
				value -= EPMConfigFlag::Debug.m_Value;
				hasAppended = true;
			}
			if ((value & EPMConfigFlag::Dist) == EPMConfigFlag::Dist) {
				if (hasAppended)
					str << " | ";
				str << "Dist";
				value -= EPMConfigFlag::Dist.m_Value;
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

	std::ostream& operator<<(std::ostream& stream, const EPMSystemFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		std::uint16_t value = flags.m_Value;
		if (value == EPMSystemFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & EPMSystemFlag::Windows) == EPMSystemFlag::Windows) {
				str << "Windows";
				value -= EPMSystemFlag::Windows.m_Value;
				hasAppended = true;
			}
			if ((value & EPMSystemFlag::Macosx) == EPMSystemFlag::Macosx) {
				if (hasAppended)
					str << " | ";
				str << "Macosx";
				value -= EPMSystemFlag::Macosx.m_Value;
				hasAppended = true;
			}
			if ((value & EPMSystemFlag::Linux) == EPMSystemFlag::Linux) {
				if (hasAppended)
					str << " | ";
				str << "Linux";
				value -= EPMSystemFlag::Linux.m_Value;
				hasAppended = true;
			}
			if ((value & EPMSystemFlag::Unix) == EPMSystemFlag::Unix) {
				if (hasAppended)
					str << " | ";
				str << "Unix";
				value -= EPMSystemFlag::Unix.m_Value;
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

	std::ostream& operator<<(std::ostream& stream, const EPMToolsetFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		std::uint16_t value = flags.m_Value;
		if (value == EPMToolsetFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & EPMToolsetFlag::MSVC) == EPMToolsetFlag::MSVC) {
				str << "MSVC";
				value -= EPMToolsetFlag::MSVC.m_Value;
				hasAppended = true;
			}
			if ((value & EPMToolsetFlag::Clang) == EPMToolsetFlag::Clang) {
				if (hasAppended)
					str << " | ";
				str << "Clang";
				value -= EPMToolsetFlag::Clang.m_Value;
				hasAppended = true;
			}
			if ((value & EPMToolsetFlag::GCC) == EPMToolsetFlag::GCC) {
				if (hasAppended)
					str << " | ";
				str << "GCC";
				value -= EPMToolsetFlag::GCC.m_Value;
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

	std::ostream& operator<<(std::ostream& stream, const EPMPlatformFlags& flags) {
		std::ostringstream str;
		bool hasAppended = false;

		std::uint16_t value = flags.m_Value;
		if (value == EPMPlatformFlag::Unknown) {
			str << "Unknown";
		} else {
			if ((value & EPMPlatformFlag::X86) == EPMPlatformFlag::X86) {
				str << "X86";
				value -= EPMPlatformFlag::X86.m_Value;
				hasAppended = true;
			}
			if ((value & EPMPlatformFlag::AMD64) == EPMPlatformFlag::AMD64) {
				if (hasAppended)
					str << " | ";
				str << "AMD64";
				value -= EPMPlatformFlag::AMD64.m_Value;
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