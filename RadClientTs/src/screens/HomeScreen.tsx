import {Text, View, Button, Image} from "react-native";
import * as React from "react";
//import image from "../../bike.png";

function HomeScreen({loading, timestamp, battery, onRefresh}) {
    return (
        <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}>
            {loading && (<Text>Loading</Text>)}
            {!loading && (<Text>Time: {timestamp}, battery: {battery}</Text>)}
            <Button title="Refresh" onPress={onRefresh} />
        </View>
    );
}

export { HomeScreen };