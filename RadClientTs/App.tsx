import * as React from 'react';
import { Text, View } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createDrawerNavigator } from '@react-navigation/drawer';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import HomeScreen from './src/screens/HomeScreen'
import SettingsScreen from './src/screens/SettingsScreen'
import MapScreen from './src/screens/MapScreen'
import AboutScreen from './src/screens/AboutScreen'

const Tab = createBottomTabNavigator();
const Drawer = createDrawerNavigator();

function MainScreen() {
    return (
        <Tab.Navigator initialRouteName="Home">
            <Tab.Screen name="Map" component={MapScreen} />
            <Tab.Screen name="Home" component={HomeScreen} />
            <Tab.Screen name="Settings" component={SettingsScreen} />
        </Tab.Navigator>
    );
}

export default function App() {
    return (
        <NavigationContainer>
            <Drawer.Navigator initialRouteName="User">
                <Drawer.Screen name="User" component={MainScreen} />
                <Drawer.Screen name="About" component={AboutScreen} />
            </Drawer.Navigator>
        </NavigationContainer>
    );
}