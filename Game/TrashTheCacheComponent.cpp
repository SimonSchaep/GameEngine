#include "TrashTheCacheComponent.h"
#include <imgui.h>
#include <implot.h>
#include <chrono>
#include <numeric>
#include <iostream>
#include <algorithm>

//feedback:
//use vector instead of array
//use std algorithms instead of manual taking avg
//take avg of all elements except min and max
//useful -> vector<int>{vector<int>(count,value)}



void TrashTheCacheComponent::Initialize()
{
}

void TrashTheCacheComponent::Update()
{

}

void TrashTheCacheComponent::RenderUI()
{
    ImGui::Begin("Exercise 1");
    ImGui::SetWindowSize({280,300}, ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos({20,100}, ImGuiCond_FirstUseEver);

    static int countEx1{10};
    ImGui::InputInt("# samples", &countEx1);
    if (ImGui::Button("Trash the cache"))
    {
        MeasureExercise1(countEx1);
    }

    if (m_Ex1Measurements.size() != 0)
    {
        if (ImPlot::BeginPlot("Ex1", {-1,100}, ImPlotFlags_NoTitle))
        {
            const int minY{ *std::min_element(m_Ex1Measurements.begin(), m_Ex1Measurements.end()) };
            const int maxY{ *std::max_element(m_Ex1Measurements.begin(), m_Ex1Measurements.end()) };

            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, m_LabelValues.front(), m_LabelValues.back());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, double(minY), double(maxY));
            ImPlot::SetupAxisTicks(ImAxis_X1, m_LabelValues.data(), int(m_Labels.size()), m_Labels.data());
            
            ImPlot::SetupFinish();

            ImPlot::PlotLine("Ex1Line", m_Ex1Measurements.data(), int(m_Ex1Measurements.size()), 1.0, 0.0, ImPlotFlags_NoTitle);
            ImPlot::EndPlot();
        }
    }    

    ImGui::End();

    ImGui::Begin("Exercise 2");
    ImGui::SetWindowSize({280,300}, ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos({340,100}, ImGuiCond_FirstUseEver);

    static int countEx2{ 10 };
    ImGui::InputInt("# samples", &countEx2);
    if (ImGui::Button("Trash the cache with GameObject3D"))
    {
        MeasureExercise2(countEx2);
    }

    if (m_Ex2Measurements.size() != 0)
    {
        if (ImPlot::BeginPlot("Ex2", { -1,100 }, ImPlotFlags_NoTitle))
        {
            const int minY{ *std::min_element(m_Ex2Measurements.begin(), m_Ex2Measurements.end()) };
            const int maxY{ *std::max_element(m_Ex2Measurements.begin(), m_Ex2Measurements.end()) };

            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, m_LabelValues.front(), m_LabelValues.back());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, double(minY), double(maxY));
            ImPlot::SetupAxisTicks(ImAxis_X1, m_LabelValues.data(), int(m_Labels.size()), m_Labels.data());
            ImPlot::SetupFinish();

            ImPlot::PlotLine("Ex2Line", m_Ex2Measurements.data(), int(m_Ex2Measurements.size()), 1.0, 0.0, ImPlotFlags_NoTitle);
            ImPlot::EndPlot();
        }
    }

    if (ImGui::Button("Trash the cache with GameObject3DAlt"))
    {
        MeasureExercise2Alt(countEx2);
    }

    if (m_Ex2AltMeasurements.size() != 0)
    {
        if (ImPlot::BeginPlot("Ex2Alt", { -1,100 }, ImPlotFlags_NoTitle))
        {
            const int minY{ *std::min_element(m_Ex2AltMeasurements.begin(), m_Ex2AltMeasurements.end()) };
            const int maxY{ *std::max_element(m_Ex2AltMeasurements.begin(), m_Ex2AltMeasurements.end()) };

            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, m_LabelValues.front(), m_LabelValues.back());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, double(minY), double(maxY));
            ImPlot::SetupAxisTicks(ImAxis_X1, m_LabelValues.data(), int(m_Labels.size()), m_Labels.data());
            ImPlot::SetupFinish();

            ImPlot::PlotLine("Ex2AltLine", m_Ex2AltMeasurements.data(), int(m_Ex2AltMeasurements.size()), 1.0, 0.0, ImPlotFlags_NoTitle);
            ImPlot::EndPlot();
        }
    }

    if (m_Ex2Measurements.size() != 0 && m_Ex2AltMeasurements.size() != 0)
    {
        ImGui::Text("Combined:");

        if (ImPlot::BeginPlot("Combined", { -1,100 }, ImPlotFlags_NoTitle))
        {
            const int minY{ std::min(*std::min_element(m_Ex2Measurements.begin(), m_Ex2Measurements.end()), *std::min_element(m_Ex2AltMeasurements.begin(), m_Ex2AltMeasurements.end())) };
            const int maxY{ std::max(*std::max_element(m_Ex2Measurements.begin(), m_Ex2Measurements.end()), *std::max_element(m_Ex2AltMeasurements.begin(), m_Ex2AltMeasurements.end())) };

            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, m_LabelValues.front(), m_LabelValues.back());
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, double(minY), double(maxY));
            ImPlot::SetupAxisTicks(ImAxis_X1, m_LabelValues.data(), int(m_Labels.size()), m_Labels.data());
            ImPlot::SetupFinish();

            ImPlot::PlotLine("Ex2Line", m_Ex2Measurements.data(), int(m_Ex2Measurements.size()), 1.0, 0.0, ImPlotFlags_NoTitle);
            ImPlot::PlotLine("Ex2AltLine", m_Ex2AltMeasurements.data(), int(m_Ex2AltMeasurements.size()), 1.0, 0.0, ImPlotFlags_NoTitle);
            ImPlot::EndPlot();
        }
    }

    

    ImGui::End();
}

void TrashTheCacheComponent::MeasureExercise1(int count)
{
    bool firstMeasurent{m_Ex1Measurements.size() == 0};

    std::vector<std::vector<int>> measurementsGroup{};

    //do measurements 'count' times
    for (int mNum{}; mNum < count; ++mNum)
    {
        std::vector<int> measurements{};

        int arrayLength = 10'000'000;
        int* pArray = new int[arrayLength];

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < arrayLength; i += stepsize)
            {
                pArray[i] *= 2;
            }
            int measurement = int(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count());
            measurements.push_back(measurement);
        }

        delete[] pArray;

        measurementsGroup.push_back(measurements);
    }

    //store avg
    for (size_t i{}; i < 11; ++i)
    {
        int sum{};
        for (auto& measurements : measurementsGroup)
        {
            sum += measurements[i];
        }

        if (firstMeasurent) m_Ex1Measurements.push_back(int(sum / measurementsGroup.size()));
        else m_Ex1Measurements[i] = int(sum / measurementsGroup.size());
    }
}

void TrashTheCacheComponent::MeasureExercise2(int count)
{
    bool firstMeasurent{ m_Ex2Measurements.size() == 0 };

    std::vector<std::vector<int>> measurementsGroup{};

    //do measurements 'count' times
    for (int mNum{}; mNum < count; ++mNum)
    {
        std::vector<int> measurements{};

        int arrayLength = 10'000'000;
        GameObject3D* pArray = new GameObject3D[arrayLength];

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < arrayLength; i += stepsize)
            {
                pArray[i].ID = 0;
            }
            int measurement = int(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count());
            measurements.push_back(measurement);
        }

        delete[] pArray;

        measurementsGroup.push_back(measurements);
    }

    //store avg
    for (size_t i{}; i < 11; ++i)
    {
        int sum{};
        for (auto& measurements : measurementsGroup)
        {
            sum += measurements[i];
        }

        if (firstMeasurent) m_Ex2Measurements.push_back(int(sum / measurementsGroup.size()));
        else m_Ex2Measurements[i] = int(sum / measurementsGroup.size());
    }
}

void TrashTheCacheComponent::MeasureExercise2Alt(int count)
{
    bool firstMeasurent{ m_Ex2AltMeasurements.size() == 0 };

    std::vector<std::vector<int>> measurementsGroup{};

    //do measurements 'count' times
    for (int mNum{}; mNum < count; ++mNum)
    {
        std::vector<int> measurements{};

        int arrayLength = 10'000'000;
        GameObject3DAlt* pArray = new GameObject3DAlt[arrayLength];

        for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
        {
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < arrayLength; i += stepsize)
            {
                pArray[i].ID = 0;
            }
            int measurement = int(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count());
            measurements.push_back(measurement);
        }

        delete[] pArray;

        measurementsGroup.push_back(measurements);
    }

    //store avg
    for (size_t i{}; i < 11; ++i)
    {
        int sum{};
        for (auto& measurements : measurementsGroup)
        {
            sum += measurements[i];
        }

        if (firstMeasurent) m_Ex2AltMeasurements.push_back(int(sum / measurementsGroup.size()));
        else m_Ex2AltMeasurements[i] = int(sum / measurementsGroup.size());
    }
}


