import {Text, View, Button, Image, StyleSheet} from "react-native";
import * as React from "react";
import { bike } from "../images";

const styles = StyleSheet.create({
        tinyLogo: {
        width: '90%',
        resizeMode: 'contain'
    }
});

function HomeScreen({loading, timestamp, battery, onRefresh}) {
    return (
        <View style={{ flex: 1, justifyContent: 'center', alignItems: 'center' }}>
            <Image
                style={styles.tinyLogo}
                source={ bike }
            />
            {loading && (<Text>Loading</Text>)}
            {!loading && (<Text>Time: {timestamp}, battery: {battery}</Text>)}
            <Button title="Refresh" onPress={onRefresh} />
        </View>
    );
}

export { HomeScreen };