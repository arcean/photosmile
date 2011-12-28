import QtQuick 1.1
import com.nokia.meego 1.0

Image {
    source: "image://theme/meegotouch-separator-" + (theme.inverted ?  "inverted-" : "")+ "background-horizontal"
}
