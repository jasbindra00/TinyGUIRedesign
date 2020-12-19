#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <SFML/Graphics/Transformable.hpp>
#include <string>
#include <functional>

namespace tiny
{
	enum class State : std::size_t
	{
		Neutral = 0, Clicked, Focused
	};


	struct ElementInfo
	{
		sf::FloatRect global_bounds;
	};
	class Raster
	{
	private:
		using RasterCalibrator = std::function<void(sf::Drawable&, ElementInfo)>;

		std::unique_ptr<sf::Drawable> drawable;
		RasterCalibrator calibrator;
		std::string name;
	public:
		Raster(const std::string& name, std::unique_ptr<sf::Drawable>&& drawable, RasterCalibrator calibrator)
			:drawable(std::move(drawable)),
			calibrator(std::move(calibrator)),
			name(name)
		{
		}
		inline const std::string& getName() const noexcept
		{
			return name;
		}
		void calibrateRaster(const ElementInfo& info)
		{
			calibrator(*drawable.get(), info);
		}
		void rasterize(sf::RenderTarget& target, sf::RenderStates element_render_state) const
		{
			target.draw(*drawable, element_render_state);
		}
	};
	class Style //Exposed to user.
	{
	private:
		std::vector<Raster> rasters;
		bool redraw_required;
		std::pair<bool, decltype(rasters)::iterator> getComponent(const std::string& name)
		{
			auto found = std::find_if(rasters.begin(), rasters.end(), [&name](const Raster& raster) {return raster.getName() == name; });
			return (found == rasters.end()) ? std::make_pair(false, found) : std::make_pair(true, found);
		}
	public:
		template<typename T>
		const T const* getComponent(const std::string& name)
		{
			auto found_component{ getComponent(name) };
			if (!found_component.first) return nullptr;
			return dynamic_cast<T*>(found_component.second->second.get());
		}
		template<typename T>
		bool setComponent(const std::string& name, std::unique_ptr<T>&& arg)
		{
			auto found_component{ getComponent(name) };
			if (!found_component.first) return false;
			found_component.second->second = std::move(arg);
			redraw_required = true;
			return true;
		}
		void rasterize(sf::RenderTarget& target, sf::RenderStates element_state) const
		{
			std::for_each(rasters.begin(), rasters.end(), [&target, &element_state](const Raster& raster) {raster.rasterize(target, element_state); });
		}
	};
	class Visual //Not exposed to the user.
	{
	private:
		std::vector<std::unique_ptr<Style>> state_styles;
	public:
		Visual()
		{
		}
		void Update() 
		{

		}
	};

int main()
{
	sf::RenderWindow target(sf::VideoMode(500, 500), "fsodjf", sf::Style::Default);
	sf::RectangleShape s;
	s.setSize(sf::Vector2f(100, 200));
	std::unique_ptr<sf::Text> e = std::make_unique<sf::Text>();
	while (target.isOpen())
	{
		sf::Event evnt;
		while (target.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::EventType::Closed:
			{
				target.close();
				break;
			}
			}
		}
		target.clear();
		target.draw(s, transform);
		target.display();

	}

}





class Drawable : public sf::Drawable
{
private:

public:

	void GetTransform()
	{
	}
};






