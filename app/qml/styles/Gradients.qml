pragma Singleton
import QtQuick 2.0

QtObject {
    property Gradient gd1: Gradient{
        GradientStop { position: 0.0; color: '#75C5F0' }
        GradientStop { position: 0.33; color: '#0093DD' }
        GradientStop { position: 0.66; color: '#007CC3' }
        GradientStop { position: 1.0; color: '#75C5F0' }
    }
    property Gradient gd2:Gradient{
        GradientStop { position: 0.0; color: '#EF9BA0' }
        GradientStop { position: 1.0; color: '#8E84b7' }
    }
    property Gradient gd3:Gradient{
        GradientStop { position: 0.0; color: '#615d5c' }
        GradientStop { position: 1.0; color: '#615d5c' }
    }
    property Gradient gd4:Gradient{
        GradientStop { position: 0.0; color: '#FFFFFF' }
        GradientStop { position: 1.0; color: '#FFFFFF' }
    }
    property Gradient gd5:Gradient{
        GradientStop { position: 0.0; color: '#FFFFFF' }
        GradientStop { position: 0.75; color: '#C1C1C1' }
        GradientStop { position: 1.0; color: '#FFFFFF' }
    }
    property Gradient gd6:Gradient{
        GradientStop { position: 0.0; color: '#3A3734' }
        GradientStop { position: 0.5; color: '#817F7F' }
        GradientStop { position: 1.0; color: '#3A3734' }
    }
    property Gradient gd_alpha:Gradient{
        GradientStop { position: 0.0; color: Qt.rgba(0.0, 0.0, 0.0, 0.0) }
        GradientStop { position: 1.0; color: Qt.rgba(0.0, 0.0, 0.0, 0.0) }
    }
}
