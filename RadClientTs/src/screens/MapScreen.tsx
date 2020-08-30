import React, { useState } from "react";

import MapView from "react-native-maps";

function MapScreen() {
    return (
        <MapView style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}
            initialRegion={{
                latitude: 52.520008,
                longitude: 13.404954,
                latitudeDelta: 0.0922,
                longitudeDelta: 0.0421,
            }}
        />
    );
}

export default MapScreen;