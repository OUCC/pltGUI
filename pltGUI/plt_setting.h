#pragma once
#include "my_gui.h"


class Pulldown
{
public:

	Pulldown() = default;

	Pulldown(const Array<String>& items,const size_t& index = 0, const Point& pos = { 0,0 })
		: m_items{ items }
		, m_rect{ pos, 0, (FontAsset(U"main").height() + m_padding.y * 2) }
	{
		for (const auto& item : m_items)
		{
			m_rect.w = Max(m_rect.w, static_cast<int32>(FontAsset(U"main")(item).region().w));
		}
		m_rect.w += (m_padding.x * 2 + m_downButtonSize);
		m_index = Clamp(index, size_t(0), items.size());
	}

	bool isEmpty() const
	{
		return m_items.empty();
	}

	Pulldown* update()
	{
		if (isEmpty())
		{
			return this;
		}

		if (m_nextClose) {
			m_isOpen = (not m_isOpen);
			m_nextClose = false;
		}

		if (m_rect.leftClicked())
		{
			m_nextClose = true;
		}

		Point pos = m_rect.pos.movedBy(0, m_rect.h);

		if (m_isOpen)
		{
			for (auto i : step(m_items.size()))
			{
				if (const Rect rect{ pos, m_rect.w, m_rect.h };
					rect.leftClicked())
				{
					m_index = i;
					m_nextClose = true;
					break;
				}

				pos.y += m_rect.h;
			}
		}
		return this;
	}

	void draw() const
	{
		m_rect.draw();

		if (isEmpty())
		{
			return;
		}

		m_rect.drawFrame(1, 0, m_isOpen ? UIColor::Accent : UIColor::frame());

		Point pos = m_rect.pos;

		FontAsset(U"main")(m_items[m_index]).draw(pos + m_padding, UIColor::text());

		Triangle{ (m_rect.x + m_rect.w - m_downButtonSize / 2.0 - m_padding.x), (m_rect.y + m_rect.h / 2.0),
			(m_downButtonSize * 0.5), 180_deg }.draw(UIColor::text());

		pos.y += m_rect.h;

		if (m_isOpen)
		{
			const Rect backRect{ pos, m_rect.w, (m_rect.h * m_items.size()) };

			backRect.drawShadow({ 1, 1 }, 4, 1).draw();

			for (const auto& item : m_items)
			{
				if (const Rect rect{ pos, m_rect.size };
					rect.mouseOver())
				{
					rect.draw(UIColor::Accent.lerp(UIColor::Base,0.5));
				}

				FontAsset(U"main")(item).draw((pos + m_padding), Palette::Black);

				pos.y += m_rect.h;
			}

			backRect.drawFrame(1, 0, Palette::Gray);
		}
	}

	Pulldown* setPos(const Point& pos)
	{
		m_rect.setPos(pos);
		return this;
	}

	const Rect& getRect() const
	{
		return m_rect;
	}

	size_t getIndex() const
	{
		return m_index;
	}

	String getItem() const
	{
		if (isEmpty())
		{
			return{};
		}

		return m_items[m_index];
	}

	void setIndex(const size_t& index) {
		if (InRange(index, size_t(0), m_items.size())) {
			m_index = index;
		}
	}

	bool isOpen() {
		return m_isOpen;
	}

private:

	Array<String> m_items;

	size_t m_index = 0;

	Size m_padding{ 6, 2 };

	Rect m_rect;

	int32 m_downButtonSize = 16;

	bool m_isOpen = false;

	bool m_nextClose = false;
};


class PltKey {
public:
	Array<String> poslist{ U"top left",U"top right",U"bottom left", U"bottom right", U"top outside", U"bottom outside" };
	Pulldown pos{ poslist, 1 };
	bool box;
};

class Terminal {
public:
	struct Info{
		String display;
		String ext;
		String command;
		bool canLoad;
		String defSize;
	};
	Array<Info> infolist;
	Terminal(const Array<Terminal::Info>& infos) {
		infolist = infos;
		ext = infolist[0].ext;
		Array<String> dispset;
		for (Info& i : infolist)
			dispset << i.display;
		pd = Pulldown{ dispset };
	};
	inline static String ext;
	Info getInfo() {
		return infolist[pd.getIndex()];
	};
	Pulldown pd;
};

/// @brief タイトルなどグラフ全体の設定
class WholeSetting {
public:
	Terminal terminal{ Array<Terminal::Info>{
		{U"pngcairo",U"png",U"pngcairo enhanced",true, U"640 , 480"},
		{U"gif",U"gif",U"gif enhanced", true, U"640 , 480"},
		{U"svg *",U"svg",U"svg enhanced", false, U"640 , 480"},
		{U"pdfcairo *",U"pdf",U"pdfcairo enhanced",false, U"5in , 3in"},
	}};
	WithBool<MyGUI::TextAreaEditState> sizex;
	WithBool<MyGUI::TextAreaEditState> sizey;
	WithBool<MyGUI::TextAreaEditState> title;
	WithBool<MyGUI::TextAreaEditState> xlabel;
	WithBool<MyGUI::TextAreaEditState> ylabel;
	WithBool<MyGUI::TextAreaEditState> xrange_min;
	WithBool<MyGUI::TextAreaEditState> xrange_max;
	WithBool<MyGUI::TextAreaEditState> yrange_min;
	WithBool<MyGUI::TextAreaEditState> yrange_max;
	WithBool<void> logscale_x;
	WithBool<void> logscale_y;
	WithBool<void> logscale_x_exp_not;//exponential notation
	WithBool<void> logscale_y_exp_not;//exponential notation
	WithBool<MyGUI::TextAreaEditState> sample;
	WithBool<PltKey> key{ true };
	WithBool<MyGUI::TextAreaEditState> xtics;
	WithBool<MyGUI::TextAreaEditState> ytics;
	WithBool<void> grid;
	WithBool<FilePath> loadfile;
};



/// @brief 色などグラフごとの設定
class GraphSetting {
public:
	GraphSetting operator = (const GraphSetting& other) {
		graph_index = other.graph_index;
		graph_func = other.graph_func;
		graph_data = other.graph_data;
		using_x = other.using_x;
		using_y = other.using_y;
		title = other.title;
		linecolor = other.linecolor;
		withlines = other.withlines;
		linewidth = other.linewidth;
		linetype = other.linetype;
		withpoints = other.withpoints;
		pointsize = other.pointsize;
		pointtype = other.pointtype;
		return *this;
	}

	int graph_index = 0;
	WithBool<MyGUI::TextAreaEditState> graph_func;
	WithBool<MyGUI::TextAreaEditState> graph_data;
	WithBool<MyGUI::TextAreaEditState> using_x;
	WithBool<MyGUI::TextAreaEditState> using_y;
	WithBool<MyGUI::TextAreaEditState> title;
	WithBool<HSV> linecolor;
	WithBool<void> withlines;
	WithBool<MyGUI::TextAreaEditState> linewidth;
	WithBool<MyGUI::TextAreaEditState> linetype;
	WithBool<void> withpoints;
	WithBool<MyGUI::TextAreaEditState> pointsize;
	WithBool<MyGUI::TextAreaEditState> pointtype;
};
