<script lang="ts">
	import { onMount, onDestroy } from 'svelte';
	import { user } from '$lib/stores/user';
	import { page } from '$app/state';
	import { notifications } from '$lib/components/toasts/notifications';
	import SettingsCard from '$lib/components/SettingsCard.svelte';
	import Light from '~icons/tabler/bulb';
	import Info from '~icons/tabler/info-circle';
	import Save from '~icons/tabler/device-floppy';
	import Reload from '~icons/tabler/reload';
	import { socket } from '$lib/stores/socket';
	import type { ScaleState } from '$lib/types/models';

	let scaleState: ScaleState = $state({
		weight: -42.0
	});

	async function getWeight() {
		try {
			const response = await fetch('/rest/scaleState', {
				method: 'GET',
				headers: {
					Authorization: page.data.features.security ? 'Bearer ' + $user.bearer_token : 'Basic',
					'Content-Type': 'application/json'
				}
			});
			const json_resp = await response.json();
			scaleState.weight = json_resp['weight']
		} catch (error) {
			console.error('Error:', error);
		}
		return;
	}

	onMount(() => {
		socket.on<ScaleState>('scale', (data) => {
			scaleState = data;
		});
		getWeight();
	});

	async function tare() {
		try {
			const response = await fetch('/rest/lightState', {
				method: 'POST',
				headers: {
					Authorization: page.data.features.security ? 'Bearer ' + $user.bearer_token : 'Basic',
					'Content-Type': 'application/json'
				},
				body: JSON.stringify({ /* TODO */ })
			});
			if (response.status == 200) {
				notifications.success('Tare send.', 3000);
				const json_resp = await response.json();
				// TODO
			} else {
				notifications.error('User not authorized.', 3000);
			}
		} catch (error) {
			console.error('Error:', error);
		}
	}
</script>

<SettingsCard collapsible={false}>
	{#snippet icon()}
		<Light class="lex-shrink-0 mr-2 h-6 w-6 self-end" />
	{/snippet}

	{#snippet title()}
		<span>Scale State</span>
	{/snippet}
	<div class="w-full">
		<h1 class="text-xl font-semibold">Weight</h1>

		<span class="block text-5xl font-bold text-right">{scaleState.weight.toFixed(1)}g</span>
		<div class="grow"></div>
		<button class="btn btn-primary inline-flex items-center" onclick={tare}
			><Save class="mr-2 h-5 w-5" /><span>Tare</span></button
		>
	</div>
</SettingsCard>
