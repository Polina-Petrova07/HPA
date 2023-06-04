#pragma once


#include <Windows.h>
#include <wincodec.h>
#include <gdiplus.h>
#include <vector>
#include "map.h"
#include "tiles.h"
std::vector<int> read_image(const TCHAR* filename) {
    // белый - можно пройти
    // черный - нельзя
    std::vector<int> impass;

    Gdiplus::GdiplusStartupInput gdiplusStrtupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStrtupInput, NULL);
    {
        Gdiplus::Bitmap input_bmp(filename);

        auto width = input_bmp.GetWidth();
        auto height = input_bmp.GetHeight();

        for (INT i = 0; i < width; i++) {
            for (INT j = 0; j < height; j++) {
                Gdiplus::Color current_color;
                input_bmp.GetPixel(i, j, &current_color);
                if (current_color.GetValue() != Gdiplus::Color::White) {
                    int vert = findVertex(j, i, width);
                    impass.push_back(vert);
                    //input_bmp.SetPixel(i, j, Gdiplus::Color::Red);
                }
            }
        }
        impass.push_back(width);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return impass;
}
Graph read_Graph(const TCHAR* filename) {
    // белый - можно пройти
    // черный - нельзя
    Graph graph;
    std::vector<Node> nodes;
    int h = 0;
    Gdiplus::GdiplusStartupInput gdiplusStrtupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStrtupInput, NULL);
    {
        Gdiplus::Bitmap input_bmp(filename);

        auto width = input_bmp.GetWidth();
        auto height = input_bmp.GetHeight();
        h = width;

        for (INT i = 0; i < width; i++) {
            for (INT j = 0; j < height; j++) {
                Gdiplus::Color current_color;
                input_bmp.GetPixel(i, j, &current_color);
                if (current_color.GetValue() == Gdiplus::Color::White) {
                    int vert = findVertex(j, i, width);
                    Node currNode(vert, 0);
                    nodes.push_back(currNode);
                }
            }
        }

    }
    for (int i = 0; i < nodes.size(); i++) {
        graph.addNodeAndEdges(nodes[i], h);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return graph;
}
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}
void draw_path(Node start, Node goal, std::vector<Node> path, int size, const TCHAR* filename_source, const TCHAR* filename_out) {
    Gdiplus::GdiplusStartupInput gdiplusStrtupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStrtupInput, NULL);
    {
        std::pair<INT, INT> currentCord;
        Gdiplus::Bitmap input_bmp(filename_source);
        for (int i = 0; i < path.size(); i++) {   
            currentCord = findPos(size, path[i].getId());
            input_bmp.SetPixel(currentCord.second, currentCord.first, Gdiplus::Color::Red);
        }

        currentCord = findPos(size, start.getId());
        input_bmp.SetPixel(currentCord.second, currentCord.first, Gdiplus::Color::Blue);

        currentCord = findPos(size, goal.getId());
        input_bmp.SetPixel(currentCord.second, currentCord.first, Gdiplus::Color::Blue);

        CLSID pngClsid;
        GetEncoderClsid(TEXT("image/bmp"), &pngClsid);
        input_bmp.Save(filename_out, &pngClsid, NULL);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
}
void draw_pixel(int x, int y, const TCHAR* filename_source, const TCHAR* filename_out) {
    Gdiplus::GdiplusStartupInput gdiplusStrtupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStrtupInput, NULL);
    {
        Gdiplus::Bitmap input_bmp(filename_source);
        input_bmp.SetPixel(y, x, Gdiplus::Color::Yellow);

        CLSID pngClsid;
        GetEncoderClsid(TEXT("image/bmp"), &pngClsid);
        input_bmp.Save(filename_out, &pngClsid, NULL);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

