#include "Exercise1Component.h"
#include "imgui.h"
#include "implot.h"

void Exercise1Component::Initialize()
{
	

}

void Exercise1Component::Update()
{
	
}

void Exercise1Component::Render() const
{
    int   bar_data[11];
    float x_data[1000];
    float y_data[1000];

    ImGui::Begin("My Window");

    if (ImPlot::BeginPlot("My Plot"))
    {
        ImPlot::PlotBars("My Bar Plot", bar_data, 11);
        ImPlot::PlotLine("My Line Plot", x_data, y_data, 1000);
        ImPlot::EndPlot();
    }

    ImGui::End();
}
