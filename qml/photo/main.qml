import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow
    initialPage: mainPage
    showStatusBar: false

    property int _SMALL_FONT_SIZE: 18
    property int _STANDARD_FONT_SIZE: 24
    property int _LARGE_FONT_SIZE: 40
    property string _TEXT_COLOR: theme.inverted ? "white" : "black"
    property string _ICON_LOCATION: "/usr/share/themes/blanco/meegotouch/icons/"
    property string _ACTIVE_COLOR: "color11"

    Component.onCompleted: {
        theme.inverted = true;
    }

    platformStyle: PageStackWindowStyle {
            background: appWindow.inPortrait ? "qrc:/data/background-portrait.png" : "qrc:/data/background.png"
            backgroundFillMode: Image.Tile
        }

    MainPage {
        id: mainPage
    }

    SettingsPage {
        id: settingsPage
    }
}
