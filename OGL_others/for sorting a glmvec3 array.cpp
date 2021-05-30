sort (windowPositions.begin(), windowPositions.end(), [&camPosition](const vec3& winPos1, const vec3& winPos2) 
{
        return length(camPosition - winPos1) >= length(camPosition - winPos2);
});

for (auto& position: windowPositions) 
{
    // draw window.
}