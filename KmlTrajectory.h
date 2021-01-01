#ifndef KML_TRAJECTORY
#define KML_TRAJECTORY


#include "KmlAbstractItem.h"
#include "KmlSimplePoint.h"
#include "KmlAltitudeMode.h"

#include <vector>
#include <functional>
#include <cmath>


namespace Kml
{
using Function = std::function<double(double)>;

class Trajectory : public AbstractItem
{
	public:
		Trajectory(const std::string &name, const std::string &styleUrl = {}, AltitudeMode altMode = AltitudeMode::Absolute) : AbstractItem{name}
		{
			this->setStyleUrl(styleUrl);
			m_altMode = altMode;
		};

		Trajectory(const Trajectory &other) = default;
		Trajectory(Trajectory &&other) = default;
		Trajectory& operator=(const Trajectory &other) = default;
		Trajectory& operator=(Trajectory &&other) = default;
		virtual ~Trajectory() = default;


		virtual std::string toString(std::size_t tabs = 0) const override final
		{
			std::string prefix(tabs,'\t');
			std::string str;
			str += prefix + "<Placemark>\n";
			str += prefix + "\t<name>" + this->name() + "</name>\n";
			str += prefix + "\t<description>" + this->description() + "</description>\n";
			str += prefix + "\t<styleUrl>" + this->styleUrl() + "</styleUrl>\n";
			str += prefix + "\t<visible>" + (this->isVisible() ? "1" : "0") + "</visible>\n";
			str += prefix + "\t<LineString>\n";
			str += prefix + "\t\t" + altitudeModeToStr(m_altMode) + "\n";
			str += prefix + "\t\t<tessellate>1</tessellate>\n";
			str += prefix + "\t\t<coordinates>\n";
			for (const SimplePoint &p : m_points) {str += prefix + "\t\t\t" + std::to_string(p.lon) + "," + std::to_string(p.lat) + "," + std::to_string(p.alt) + "\n";}
			str += prefix + "\t\t</coordinates>\n";
			str += prefix + "\t</LineString>\n";
			str += prefix + "</Placemark>\n";
			return str;
		};

		bool setPoints(double t1, double t2, double dt, Function lon, Function lat, Function alt)
		{
			if (t1 > t2 || dt <= 0) {return false;}
			int n = 1 + std::floor((t2-t1)/dt);
			m_points.clear();
			m_points.reserve(n+10);

			for (double t=t1; t<=t2; t+=dt) {m_points.emplace_back(lon(t),lat(t),alt(t));}
			return true;
		};
		void setPoints(const std::vector<SimplePoint> &points) {m_points = points;};
		void addPoint(const SimplePoint &p) {m_points.push_back(p);};

		void setAltitudeMode(AltitudeMode am) {m_altMode = am;};
		AltitudeMode altitudeMode() const {return m_altMode;};
		

	private:
		std::vector<SimplePoint> m_points;
		AltitudeMode m_altMode = AltitudeMode::Absolute;
};
}


#endif

