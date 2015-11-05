// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <Kaim/CWorldService.h>
#include <Kaim/CAsyncModule.h>
#include <Kaim/CObject.h>

namespace Kaim
{

namespace Mpd
{

/** Kaim::Mpd::CLpfManager (VTable=0x01EA0740) */
class CLpfManager : public Kaim::CWorldService
{
public:
	/** Kaim::Mpd::CLpfManager::CLpfManager_AsyncModule (VTable=0x01EA0780) */
	class CLpfManager_AsyncModule : public Kaim::CAsyncModule
	{
	public:
		virtual void vfn_0001_397794BD() = 0;
		virtual void vfn_0002_397794BD() = 0;
		virtual void vfn_0003_397794BD() = 0;
		virtual void vfn_0004_397794BD() = 0;
		virtual void vfn_0005_397794BD() = 0;
		virtual void vfn_0006_397794BD() = 0;
		virtual void vfn_0007_397794BD() = 0;
		virtual void vfn_0008_397794BD() = 0;
	};

	/** Kaim::Mpd::CLpfManager::CPolylineNode (VTable=0x01EA0838) */
	class CPolylineNode : public Kaim::CObject
	{
	public:
		/** Kaim::Mpd::CLpfManager::CPolylineNode::CEdgePassability (VTable=0x01EA0858) */
		class CEdgePassability : public Kaim::CObject
		{
		public:
			virtual void vfn_0001_9FF5CCFD() = 0;
			virtual void vfn_0002_9FF5CCFD() = 0;
		};

		virtual void vfn_0001_518AB8A8() = 0;
		virtual void vfn_0002_518AB8A8() = 0;
	};

	/** Kaim::Mpd::CLpfManager::CLpfContext (VTable=0x01EA0720) */
	class CLpfContext : public Kaim::CObject
	{
	public:
		/** Kaim::Mpd::CLpfManager::CLpfContext::CObstacleEvent (VTable=0x01EA0878) */
		class CObstacleEvent : public Kaim::CObject
		{
		public:
			virtual void vfn_0001_C749CCDC() = 0;
			virtual void vfn_0002_C749CCDC() = 0;
		};

		virtual void vfn_0001_AD44109C() = 0;
		virtual void vfn_0002_AD44109C() = 0;
		virtual void vfn_0003_AD44109C() = 0;
		virtual void vfn_0004_AD44109C() = 0;
	};

	/** Kaim::Mpd::CLpfManager::CLpfArea (VTable=0x01EA07A8) */
	class CLpfArea : public Kaim::CObject
	{
	public:
		virtual void vfn_0001_E442FE9A() = 0;
		virtual void vfn_0002_E442FE9A() = 0;
	};

	/** Kaim::Mpd::CLpfManager::CObstacleRecord (VTable=0x01EA0898) */
	class CObstacleRecord : public Kaim::CObject
	{
	public:
		virtual void vfn_0001_9EEF8DF1() = 0;
		virtual void vfn_0002_9EEF8DF1() = 0;
	};

	virtual void vfn_0001_449412FE() = 0;
	virtual void vfn_0002_449412FE() = 0;
	virtual void vfn_0003_449412FE() = 0;
	virtual void vfn_0004_449412FE() = 0;
	virtual void vfn_0005_449412FE() = 0;
	virtual void vfn_0006_449412FE() = 0;
	virtual void vfn_0007_449412FE() = 0;
};

} // namespace Mpd

} // namespace Kaim
