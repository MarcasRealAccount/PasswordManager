#include "Vma.h"
#include "Graphics/Device/Device.h"
#include "Graphics/Instance.h"

namespace Graphics::Memory {
	Vma::Vma(Device& device)
	    : m_Device(device) {
		m_Device.addChild(*this);
	}

	Vma::~Vma() {
		m_Device.removeChild(*this);
	}

	void Vma::createImpl() {
		auto& instance = m_Device.getInstance();

		VmaAllocatorCreateInfo createInfo = { {}, m_Device.getPhysicalDevice(), *m_Device, 0, nullptr, nullptr, 0, nullptr, nullptr, nullptr, *instance, instance.getApiVersion(), nullptr };
		vmaCreateAllocator(&createInfo, &m_Handle);
	}

	bool Vma::destroyImpl() {
		vmaDestroyAllocator(m_Handle);
		return true;
	}
} // namespace Graphics::Memory
