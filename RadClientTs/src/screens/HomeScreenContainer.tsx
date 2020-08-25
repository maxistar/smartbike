import React from 'react';
import { HomeScreen } from './HomeScreen';

class HomeScreenContainer extends React.Component {
    constructor(props: Readonly<{}>) {
        super(props);

        this.state = {
            loading: true
        };
    }

    async loadData() {
        const requestOptions = {
            method: 'GET',
            headers: { 'Content-Type': 'application/json' }
        };
        const response = await fetch('http://maxistar.ru:8080/api/info/latest/', requestOptions);
        const data = await response.json();
        //console.log(data);
        this.setState({ loading: false, timestamp: data.timestamp, battery: data.battery });
    }

    async componentDidMount() {
        await this.loadData();
    }

    render() {
        const { timestamp, battery, loading } = this.state;
        return (
            <HomeScreen loading={loading} timestamp={timestamp} battery={battery} onRefresh={()=>this.loadData()} />
        );
    }
}

export { HomeScreenContainer };