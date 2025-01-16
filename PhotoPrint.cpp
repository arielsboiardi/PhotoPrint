#include <SDKDDKVer.h>
#include <BX3SSU.hpp> 
#include <Arduino.hpp>  
#include <ArduinoNanoESP32.hpp>
#include <Serial.hpp>

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/tglbtn.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
 
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    asio::io_service io;
    asio::serial_port serial;
    BX3SSU *stage=nullptr;
    ArduinoNanoESP32 *controller=nullptr;

    wxChoice* PortChoice;
    wxArrayString PortList;

    void OnConnect(wxCommandEvent& event);
    void OnSelectPort(wxCommandEvent& event);
    void RefreshCOMs(wxCommandEvent& event);
    wxButton* ConnectButton;

    void OnMoveStage(wxCommandEvent& event);
    void OnStopStage(wxCommandEvent& event);
    void OnSetStageSpeed(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnStartPasses(wxCommandEvent& event);

    wxPanel *StageXYMPanel; 
    wxPanel *MultiPassPanel;
    wxTextCtrl *StageSPDCtrl;
    wxTextCtrl *StageXMCtrl;
    wxTextCtrl *StageYMCtrl;
    wxTextCtrl *PassesCtrl;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("PhotoPrint");
    frame->Show(true);
    return true;
}
 
MyFrame::MyFrame(const wxString& title): 
wxFrame(NULL, wxID_ANY, title), serial(io)
{
    // Set the frame icon
    SetIcon(wxICON(AppIcon));

    this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

    // Menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT,"&Exit\tCtrl-Q", "Exit the application");
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

    // Main window panel
    wxBoxSizer* FrameBoxSizer = new wxBoxSizer(wxVERTICAL);

    // Port selection panel
    wxPanel *PortSelectPanel = new wxPanel(this,wxID_ANY);
    wxStaticBoxSizer *PortSelectSizer = new wxStaticBoxSizer(wxHORIZONTAL,PortSelectPanel,wxT("Port Selection"));
    PortSelectPanel -> SetSizer(PortSelectSizer);

    // Port selection
    PortChoice = new wxChoice(PortSelectPanel,wxID_ANY,wxDefaultPosition,wxDefaultSize,PortList);
    PortChoice -> Bind(wxEVT_CHOICE,&MyFrame::OnSelectPort,this);

    // Connection button 
    wxButton * ReloadCOMsButton = new wxButton(PortSelectPanel,wxID_ANY,wxT("Search"),wxDefaultPosition);
    ReloadCOMsButton -> Bind(wxEVT_BUTTON,&MyFrame::RefreshCOMs,this);
    ConnectButton = new wxButton(PortSelectPanel,wxID_ANY,wxT("Connect"),wxDefaultPosition);
    ConnectButton -> Bind(wxEVT_BUTTON,&MyFrame::OnConnect,this);
    ConnectButton -> Enable(false);

    PortSelectSizer -> Add(PortChoice, 1, wxALL, 5);
    PortSelectSizer -> Add(ReloadCOMsButton, 0, wxALL, 5);
    PortSelectSizer -> Add(ConnectButton, 0, wxALL, 5);

    // Stage movement
    StageXYMPanel = new wxPanel(this,wxID_ANY);
    wxStaticBoxSizer *StageXYMSizer = new wxStaticBoxSizer(wxHORIZONTAL,StageXYMPanel,wxT("Stage Movement"));
    StageXYMPanel -> SetSizer(StageXYMSizer);

    // Stage motion controls
    wxBoxSizer* StageCtrlSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* StageSPDSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* StageXCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* StageYCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* StageButtonSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *StageSPDLabel = new wxStaticText(StageXYMPanel,wxID_ANY,wxT("Speed [um/s]"));
    StageSPDCtrl = new wxTextCtrl(StageXYMPanel,wxID_ANY,wxT("0"));
    wxButton * SetStageSpeedButton = new wxButton(StageXYMPanel,wxID_ANY,wxT("Set"),wxDefaultPosition);
    SetStageSpeedButton -> Bind(wxEVT_BUTTON,&MyFrame::OnSetStageSpeed,this);

    StageSPDSizer -> Add(StageSPDLabel, 0, wxALIGN_CENTER|wxALL, 5);
    StageSPDSizer -> Add(StageSPDCtrl, 1, wxALL, 5);
    StageSPDSizer -> Add(SetStageSpeedButton, 0, wxEXPAND|wxALL, 5);
    StageCtrlSizer -> Add(StageSPDSizer, 1, wxEXPAND);

    wxStaticText *StageXMLabel = new wxStaticText(StageXYMPanel,wxID_ANY,wxT("X [um]"));
    StageXMCtrl = new wxTextCtrl(StageXYMPanel,wxID_ANY,wxT("0"));
    wxStaticText *StageYMLabel = new wxStaticText(StageXYMPanel,wxID_ANY,wxT("Y [um]"));
    StageYMCtrl = new wxTextCtrl(StageXYMPanel,wxID_ANY,wxT("0"));
    wxButton * MoveStageButton = new wxButton(StageXYMPanel,wxID_ANY,wxT("Move"),wxDefaultPosition);
    wxButton * StopStageButton = new wxButton(StageXYMPanel,wxID_ANY,wxT("Stop"),wxDefaultPosition);
    MoveStageButton -> Bind(wxEVT_BUTTON,&MyFrame::OnMoveStage,this);
    // StopStageButton -> Bind(wxEVT_BUTTON,&MyFrame::OnStopStage,this);

    StageXCtrlSizer -> Add(StageXMLabel, 0, wxALIGN_CENTER|wxALL, 5);
    StageXCtrlSizer -> Add(StageXMCtrl, 1, wxALL, 5);
    StageYCtrlSizer -> Add(StageYMLabel, 0, wxALIGN_CENTER|wxALL, 5);
    StageYCtrlSizer -> Add(StageYMCtrl, 1, wxALL, 5);
    StageCtrlSizer -> Add(StageXCtrlSizer, 1, wxEXPAND);
    StageCtrlSizer -> Add(StageYCtrlSizer, 1, wxEXPAND);
    StageXYMSizer -> Add(StageCtrlSizer, 1, wxALL, 5);
    StageButtonSizer -> Add(MoveStageButton, 1, wxEXPAND|wxALL, 5);
    StageButtonSizer -> Add(StopStageButton, 1, wxEXPAND|wxALL, 5);
    StageXYMSizer -> Add(StageButtonSizer, 0, wxEXPAND);

    // Multi-pass panel
    MultiPassPanel = new wxPanel(this, wxID_ANY);
    wxStaticBoxSizer *MultiPassSizer = new wxStaticBoxSizer(wxHORIZONTAL, MultiPassPanel, wxT("Multi-Pass"));
    MultiPassPanel->SetSizer(MultiPassSizer);

    // Multi-pass controls
    wxStaticText *PassesLabel = new wxStaticText(MultiPassPanel, wxID_ANY, wxT("Number of Passes"));
    PassesCtrl = new wxTextCtrl(MultiPassPanel, wxID_ANY, wxT("1"));
    wxButton *StartPassesButton = new wxButton(MultiPassPanel, wxID_ANY, wxT("Start"), wxDefaultPosition);
    StartPassesButton->Bind(wxEVT_BUTTON, &MyFrame::OnStartPasses, this);

    // Add controls to sizer
    MultiPassSizer->Add(PassesLabel, 0, wxALIGN_CENTER | wxALL, 5);
    MultiPassSizer->Add(PassesCtrl, 1, wxALL, 5);
    MultiPassSizer->Add(StartPassesButton, 0, wxEXPAND | wxALL, 5);

    // Disable stage movement controls
    StageXYMPanel -> Enable(false);
    MultiPassPanel -> Enable(false);

    // Status bar
    CreateStatusBar();
    SetStatusText("Search serial devices to start");

    // Add content to main sizer
    FrameBoxSizer->Add(PortSelectPanel,0,wxEXPAND|wxALL,5);
    FrameBoxSizer->Add(StageXYMPanel,0,wxEXPAND|wxALL,5);
    FrameBoxSizer->Add(MultiPassPanel, 0, wxEXPAND | wxALL, 5);

    // Set size hints
    FrameBoxSizer -> SetSizeHints(this);

    // Set sizer
    this -> SetSizer(FrameBoxSizer);
    this -> Layout();

}
 
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("PhotoPrint was developed by Ariel S. Boiardi at SISSA BIOMAT Lab",
    "About PhotoPrint", wxOK);
}

void MyFrame::OnMoveStage(wxCommandEvent& event)
{
    int x = wxAtoi(StageXMCtrl->GetValue());
    int y = wxAtoi(StageYMCtrl->GetValue());

    wxLogStatus(wxString("Moving stage by (") + wxString::Format("%d", x) + wxString(", ") + wxString::Format("%d", y) + wxString(") um"));

    if(stage -> isConnected())
    {
        stage->move(1000*x,1000*y);
    }
    else
    {
        wxLogStatus(wxString("Stage not connected"));
    }

    wxLogStatus(wxString("Finished"));
    
}

void MyFrame::OnSetStageSpeed(wxCommandEvent &event)
{
    int speed = wxAtoi(StageSPDCtrl->GetValue());
    wxLogStatus(wxString("Setting stage speed to ") + wxString::Format("%d", speed) + wxString(" um/s"));
    if(stage -> isConnected())
    {
        stage->setSpeed(speed/10);
    }
    else
    {
        wxLogStatus(wxString("Stage not connected"));
    }
}

void MyFrame::OnStartPasses(wxCommandEvent& event)
{
    int passes = wxAtoi(PassesCtrl->GetValue());
    wxLogStatus(wxString("Starting ") + wxString::Format("%d", passes) + wxString(" passes"));

    int x = wxAtoi(StageXMCtrl->GetValue());
    int y = wxAtoi(StageYMCtrl->GetValue());

    int sign = 1;

    if(stage->isConnected())
    {
        for(int i = 0; i < passes; ++i)
        {
            wxLogStatus(wxString("Pass ") + wxString::Format("%d", i + 1));

            // Move stage   
            stage->move(sign*1000*x,sign*1000*y);

            // Change direction
            sign *= -1;
        }
        wxLogStatus("All passes completed");
    }
    else
    {
        wxLogStatus("Stage not connected");
    }
}

void MyFrame::OnConnect(wxCommandEvent &event)
{
    if(controller==nullptr)   
    {
        // Get selected port
        auto choice = PortChoice -> GetSelection();
        std::string SelectedPort = PortList[choice];
        
        // Create controller object
        controller = new ArduinoNanoESP32(io,SelectedPort);

        // Update button label
        ConnectButton->SetLabel("Disconnect");
        SetStatusText("Connected to "+PortList[PortChoice->GetSelection()]);
    }
    else
    {
        // Delete controller object
        controller->~ArduinoNanoESP32();
        controller = nullptr;

        // Update button label
        ConnectButton->SetLabel("Connect");
        SetStatusText("Connect to a port");
    }
}

void MyFrame::RefreshCOMs(wxCommandEvent &event)
{
    SetStatusText("Searching for serial devices");

    serial.close();
    Serial temp_serial(io);
    auto ports_avail = temp_serial.enumerate_ports();
    PortList.Clear();
    for (const std::string& port : ports_avail)
    {
        PortList.Add(port);
    }

    PortChoice->Clear();
    PortChoice->Append(PortList);

    if(PortList.GetCount()>0)
        SetStatusText("Select a port to connect");
    else
        SetStatusText("No serial devices found");
}

void MyFrame::OnSelectPort(wxCommandEvent &event)
{
    ConnectButton->Enable(true);
}