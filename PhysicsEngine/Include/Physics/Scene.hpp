class CScene
{
public:
    CScene(); 
    
    
    void Update(float DeltaTime );
    void Render();


protected: 
    const int m_ScreenWidth = 800; 
    const int m_ScreenHeight = 600;
    const int m_TargetFPS = 120; 
};