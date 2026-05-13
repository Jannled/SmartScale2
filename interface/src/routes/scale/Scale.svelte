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
		<span>Light State</span>
	{/snippet}
	<div class="w-full">
		<h1 class="text-xl font-semibold">Weight</h1>
		<div class="alert alert-info my-2 shadow-lg">
			<Info class="h-6 w-6 shrink-0 stroke-current" />
			<span>The form below controls the LED via the RESTful service exposed by the ESP device.</span
			>
		</div>
		<div class="flex flex-row flex-wrap justify-between gap-x-2">
			<div class="fieldset w-52">
				<label class="label cursor-pointer">
					<span class="mr-4 text-base">Light State?</span>
					<input type="checkbox" bind:checked={lightOn} class="checkbox checkbox-primary" />
				</label>
			</div>
			<div class="grow"></div>
			<button class="btn btn-primary inline-flex items-center" onclick={tare}
				><Save class="mr-2 h-5 w-5" /><span>Tare</span></button
			>
		</div>
	</div>
</SettingsCard>
