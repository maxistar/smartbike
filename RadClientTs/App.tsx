import * as React from 'react';
import { Text, View } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createDrawerNavigator } from '@react-navigation/drawer';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import { HomeScreenContainer } from './src/screens/HomeScreenContainer'
import SettingsScreen from './src/screens/SettingsScreen'
import MapScreen from './src/screens/MapScreen'
import AboutScreen from './src/screens/AboutScreen'
//import Ionicons from 'react-native-vector-icons/Ionicons';
//import Ionicons from 'react-native-vector-icons/FontAwesome5';
//import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';
import MaterialCommunityIcons from 'react-native-vector-icons/MaterialCommunityIcons';

const Tab = createBottomTabNavigator();
const Drawer = createDrawerNavigator();
/**
 * 

            <!--<Tab.Screen name="Map"
                component={MapScreen}
                options={{
                    tabBarLabel: 'Map',
                    tabBarIcon: ({ color, size }) => (
                        <MaterialCommunityIcons name="map" color={color} size={size} />
                    ),
                }}
            />-->

 */
function MainScreen() {
    return (
        <Tab.Navigator initialRouteName="Home"


        >

            <Tab.Screen
                name="Home" component={HomeScreenContainer}
                options={{
                    tabBarLabel: 'Home',
                    tabBarIcon: ({ color, size }) => (
                        <MaterialCommunityIcons name="home" color={color} size={size} />
                    ),
                }}
            />
            <Tab.Screen
                name="Settings"
                component={SettingsScreen}
                options={{
                    tabBarLabel: 'Settings',
                    tabBarIcon: ({ color, size }) => (
                        <MaterialCommunityIcons name="account" color={color} size={size} />
                    ),
                }}
            />
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